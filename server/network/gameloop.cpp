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
                   Broadcaster& broadcaster):
        gameLoopQueue(gameLoopQueue), chosenCars_(chosenCars), broadcaster_(broadcaster) {}

void GameLoop::run() {
    try {

        while (should_keep_running()) {
            processCommands();
            simulateGame();

            std::this_thread::sleep_for(Milliseconds(250));
        }
    } catch (const ClosedQueue& e) {
        return;
    }
}

void GameLoop::processCommands() {
    std::shared_ptr<Dto> command;
    while (gameLoopQueue.try_pop(command)) {
        uint8_t command_code = command->return_code();
        std::cout << "El codigo del comando popeado es" << (int)command_code << "/n";
    }
}


void GameLoop::simulateGame() {
    auto vehicle = std::make_shared<VehicleDto>(1, 1.0f, 1.0f, 0.0f);
    broadcaster_.broadcast(vehicle);
}

GameLoop::~GameLoop() {}
