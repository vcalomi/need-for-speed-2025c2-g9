#include "./network_system.h"

#include <iostream>
#include <memory>

NetworkSystem::NetworkSystem(Client& client, EventBus& bus): client_(client) {

    bus.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        std::cout << "[NetworkSystem] Sending move: " << (int)e.move << " for " << e.username
                  << std::endl;

        client_.getSenderQueue().try_push(
                std::make_shared<PlayerMoveDto>(e.username, static_cast<uint8_t>(e.move)));
    });
}
