#include "gameloop.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

#include <netinet/in.h>

#include "../common/common_codes.h"

using Clock = std::chrono::steady_clock;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

GameLoop::GameLoop(Queue<Dto>& gameLoopQueue) : 
    gameLoopQueue(gameLoopQueue) {}

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
    Dto command;
    while (gameLoopQueue.try_pop(command)) {
        
    }
}

void GameLoop::simulateGame() { }

GameLoop::~GameLoop() {}
