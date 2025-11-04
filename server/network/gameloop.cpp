#include "gameloop.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#include "../../common/Dto/vehicle.h"
#include "../../common/common_codes.h"
#include "../YamlParser.h"
#include "../constants.h"
#include "../physics/LevelCreator.h"


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
        std::cout << "gameloop incializado"
                  << "\n";
        while (should_keep_running()) {
            simulateGame();
            processCommands();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const ClosedQueue& e) {
        return;
    }
}

void GameLoop::processCommands() {
    std::shared_ptr<Dto> command;
    while (gameLoopQueue.try_pop(command)) {
        uint8_t command_code = command->return_code();
        std::cout << "El codigo del comando popeado es" << (int)command_code << "\n";
    }
}


void GameLoop::simulateGame() {
    for (int i = 0; i < maxPlayers; i++) {
        auto vehicle = std::make_shared<VehicleDto>(1, 1.0f, 1.0f, 0.0f);
        // std::cout << "GameLoop: Sending update for player " <<" at (" << vehicle->x << ", " <<
        // vehicle->y << ")" << std::endl;
        broadcaster_.broadcast(vehicle);
    }
}

GameLoop::~GameLoop() {}
