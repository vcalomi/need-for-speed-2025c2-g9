#include "./network_system.h"

#include <iostream>
#include <memory>

NetworkSystem::NetworkSystem(Client& client, EventBus& eventBus):
        client_(client), eventBus_(eventBus) {

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        if (e.move != ActionCode::IDLE) {
            std::cout << "[NetworkSystem] Sending move " << (int)e.move << " for player "
                      << e.username << std::endl;

            client_.getSenderQueue().try_push(
                    std::make_shared<PlayerMoveDto>(e.username, static_cast<uint8_t>(e.move)));
        }
    });
}
