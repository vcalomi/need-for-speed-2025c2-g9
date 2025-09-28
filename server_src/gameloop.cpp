#include "gameloop.h"
#include "../common_src/common_codes.h"
#include <netinet/in.h>
#include <cstdint>
#include <cstring>

using Clock = std::chrono::steady_clock;
using Milliseconds = std::chrono::milliseconds;

GameLoop::GameLoop(Queue<ActionCode>& commandQueue, ClientMonitor& clientMonitor) :
    commandQueue(commandQueue), clientMonitor(clientMonitor), carsWithActiveNitro(0), nitroActive(false), counterActive(0), keep_running(true) {}

// void GameLoop::run() {
//     const Milliseconds targetFrameTime(250);
//     try {
//         while(keep_running) {
//             auto frameStart = Clock::now();
//             processCommands();

//             simulateGame();
//             auto frameEnd = Clock::now();
//             auto frameDuration = std::chrono::duration_cast<Milliseconds>(frameEnd - frameStart);
//             auto sleepTime = targetFrameTime - frameDuration;
//             if (sleepTime.count() > 0) {
//                 std::this_thread::sleep_for(sleepTime);
//             }
//         }
//     } catch(const std::exception& e) {
//         return;
//     }
//     keep_running = false;
// }

void GameLoop::run() {
    try {
        while (keep_running) {
            auto frameStart = Clock::now();
            
            processCommands();
            simulateGame();
            
            auto frameEnd = Clock::now();
            auto frameDuration = std::chrono::duration_cast<Milliseconds>(frameEnd - frameStart);
            auto sleepTime = Milliseconds(250) - frameDuration;
            
            if (sleepTime.count() > 0) {
                std::this_thread::sleep_for(sleepTime);
            }
        }
    } catch (const ClosedQueue& e) {
        keep_running = false;
    }
}

void GameLoop::processCommands() {
    ActionCode action;
    while (commandQueue.try_pop(action)) {
        if (action == ActionCode::ACTIVATE_NITRO) {
            if (nitroActive) {
                continue;
            }
            carsWithActiveNitro = 1;
            nitroActive = true;
            counterActive = 0;
            broadcastNitroEvent(true);
        }
    }
}

void GameLoop::simulateGame() {
    if (nitroActive) {
        counterActive++;
        if (counterActive >= 12) {
            carsWithActiveNitro = 0;
            nitroActive = false;
            broadcastNitroEvent(false);
        }
    }
}

void GameLoop::broadcastNitroEvent(bool activated) {
    auto msg = createNitroMessage(carsWithActiveNitro, activated);
    clientMonitor.broadcast(msg);  // enviar a todos los clientes

    if (activated) {
        std::cout << "A car hit the nitro!" << std::endl;
    } else {
        std::cout << "A car is out of juice." << std::endl;
    }
}
    
std::vector<uint8_t> GameLoop::createNitroMessage(uint16_t carsWithActiveNitro, bool activated) {
    std::vector<uint8_t> msg;
    msg.resize(4);
    msg[0] = uint8_t(ActionCode::SERVER_MSG);
    uint16_t parsed = htons(carsWithActiveNitro);
    std::memcpy(&msg[1], &parsed, sizeof(parsed));
    
    msg[3] = activated ? 
        uint8_t(ActionCode::NITRO_ACTIVATED) :
        uint8_t(ActionCode::NITRO_DEACTIVATED);

    return msg;
}

void GameLoop::close() {
    // commandQueue.close();
    keep_running = false;
}

GameLoop::~GameLoop() {}