#include "gameloop.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

#include <netinet/in.h>

#include "../common_src/common_codes.h"

using Clock = std::chrono::steady_clock;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

GameLoop::GameLoop(Queue<ClientCommand>& gameLoopQueue, ClientMonitor& clientMonitor):
        gameLoopQueue(gameLoopQueue), clientMonitor(clientMonitor) {}

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
    ClientCommand command;
    while (gameLoopQueue.try_pop(command)) {
        if (command.action == ActionCode::ACTIVATE_NITRO) {
            processNitroCommand(command.clientId);
        }
    }
}

void GameLoop::processNitroCommand(int clientId) {
    auto [it, inserted] = nitroStates.try_emplace(clientId, clientId);
    NitroState& state = it->second;

    if (!state.isActive) {
        state.isActive = true;
        state.activationTime = Clock::now();

        uint16_t carsWithActiveNitro = countActiveNitro();
        broadcastNitroEvent(carsWithActiveNitro, true);
    }
}

int GameLoop::countActiveNitro() {
    return std::count_if(nitroStates.begin(), nitroStates.end(), [](const auto& pair) {
        auto& [clientId, state] = pair;
        return state.isActive;
    });
}

void GameLoop::simulateGame() {
    auto now = Clock::now();
    std::vector<int> toDeactivate;

    for (auto& [clientId, state]: nitroStates) {
        if (state.isActive) {
            auto duration = now - state.activationTime;

            if (duration >= Seconds(3)) {
                toDeactivate.push_back(state.clientId);
            }
        }
    }

    if (!toDeactivate.empty()) {
        for (int clientId: toDeactivate) {
            nitroStates[clientId].isActive = false;
            uint16_t carsWithActiveNitro = countActiveNitro();
            broadcastNitroEvent(carsWithActiveNitro, false);
        }
    }
}

void GameLoop::broadcastNitroEvent(uint16_t carsWithActiveNitro, bool activated) {
    auto msg = createNitroMessage(carsWithActiveNitro, activated);
    clientMonitor.broadcast(msg);

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

    msg[3] = activated ? uint8_t(ActionCode::NITRO_ACTIVATED) :
                         uint8_t(ActionCode::NITRO_DEACTIVATED);

    return msg;
}

GameLoop::~GameLoop() {}
