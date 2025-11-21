#include "./network_system.h"

#include <iostream>
#include <memory>

NetworkSystem::NetworkSystem(Client& client, EventBus& eventBus):
        client_(client), eventBus_(eventBus) {

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        uint8_t move_mask = static_cast<uint8_t>(e.move);
        if (move_mask != 0) {
            client_.getSenderQueue().try_push(
                    std::make_shared<PlayerMoveDto>(e.username, move_mask));
        }
    });
}
