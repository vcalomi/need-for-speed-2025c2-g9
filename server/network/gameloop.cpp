#include "gameloop.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#include "../../common/Dto/player.h"
#include "../../common/Dto/vehicle.h"
#include "../../common/Dto/player_move.h"
#include "../../common/common_codes.h"
#include "../YamlParser.h"
#include "../constants.h"
#include "../physics/LevelCreator.h"
#include "../../common/vehicle_type_utils.h"
#include "../constants.h"

using Clock = std::chrono::steady_clock;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

GameLoop::GameLoop(Queue<std::shared_ptr<Dto>>& gameLoopQueue, std::map<int, CarConfig>& chosenCars,
                   Broadcaster& broadcaster, int maxPlayers):
        gameLoopQueue(gameLoopQueue),
        chosenCars_(chosenCars),
        broadcaster_(broadcaster),
        maxPlayers(maxPlayers) {}

void GameLoop::run() {
    try {
        setup.emplace("../server/physics/Levels",
              "../server/vehicles_specs/vehicle_specs.yaml",
              chosenCars_);
        sendInitialPlayersCars();
        while (should_keep_running()) {
            simulateGame();
            processCommands();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const ClosedQueue& e) {
        return;
    }
}

void GameLoop::sendInitialPlayersCars() {
    for (const auto& [playerId, cfg] : chosenCars_) {
        VehicleTipe vt;
        try {
            vt = toVehicleTipe(cfg.carType);
        } catch (const std::exception& e) {
            std::cerr << "[sendInitialPlayersCars] player " << playerId
                    << " has invalid carType" << cfg.carType
                    << " (" << e.what() << ")\n";
            continue;
        }

        auto dto = std::make_shared<PlayerDto>(static_cast<uint8_t>(playerId), vt);
        broadcaster_.broadcast(dto);
    }
}

void GameLoop::simulateGame() {

    for (auto& [player_id, vehicle]: setup.value().getVehicleMap()) {
        float x, y, angle;
        vehicle->getPosition(x, y, angle);
        auto dto = std::make_shared<VehicleDto>(player_id, x, y, angle);
        broadcaster_.broadcast(dto);
        //std::cout << "mandando pos vehicle /n";
    }
}


void GameLoop::processCommands() {
    std::shared_ptr<Dto> command;
    while (gameLoopQueue.try_pop(command)) {
        std::cerr << "[CHUPETE]: \n";
        handlerProcessCommand(command);
    }
}

Vehicle* GameLoop::getVehicleByPlayer(int player_id) {
    const auto& map = setup->getVehicleMap();
    auto it = map.find(player_id);
    return (it == map.end()) ? nullptr : it->second.get();
}

void GameLoop::handlerProcessCommand(std::shared_ptr<Dto> command) {
    Vehicle* vehicle = getVehicleByPlayer(command->get_cliente_id());
    
    switch (static_cast<ActionCode>(command->return_code())) {
        case ActionCode::SEND_PLAYER_MOVE: {
            auto player_move_dto = std::dynamic_pointer_cast<PlayerMoveDto>(command);
            switch(static_cast<ActionCode>(player_move_dto->move)){
                case ActionCode::TURN_RIGHT:   vehicle->turn(TurnDir::Right);   break;
                case ActionCode::TURN_LEFT:    vehicle->turn(TurnDir::Left);    break;
                case ActionCode::ACCELERATE:   vehicle->accelerate();           break;
                case ActionCode::BRAKE:        vehicle->brake();                break;
                default:
                std::cerr << "[GameLoop] unknown command: " << command->return_code() << "\n";
                    break;
            }
            break;
        }
        default:
            std::cerr << "[GameLoop] unknown command: " << command->return_code() << "\n";
            break;
    }
}

GameLoop::~GameLoop() {}
