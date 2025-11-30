#include "./network_system.h"

#include <iostream>
#include <memory>

#include "../../common/Dto/end_race.h"
#include "../../common/Dto/infinite_health.h"
#include "../../common/Dto/player_left.h"
#include "../../events/cheat_end_race_event.h"
#include "../../events/infinite_health_event.h"
#include "../../events/player_left_event.h"

NetworkSystem::NetworkSystem(Client& client, EventBus& eventBus):
        client_(client), eventBus_(eventBus) {

    eventBus_.Subscribe<PlayerMoveEvent>([this](const PlayerMoveEvent& e) {
        uint8_t move_mask = static_cast<uint8_t>(e.move);
        if (move_mask != 0) {
            client_.getSenderQueue().try_push(
                    std::make_shared<PlayerMoveDto>(e.username, move_mask));
        }
    });

    eventBus_.Subscribe<PlayerLeftEvent>([this](const PlayerLeftEvent& e) {
        client_.getSenderQueue().try_push(std::make_shared<PlayerLeftDto>(e.username));
    });

    eventBus_.Subscribe<InfiniteHealthEvent>([this](const InfiniteHealthEvent& e) {
        client_.getSenderQueue().try_push(std::make_shared<InfiniteHealthDto>(e.username));
    });

    eventBus_.Subscribe<CheatEndRaceEvent>([this](const CheatEndRaceEvent& e) {
        client_.getSenderQueue().try_push(std::make_shared<EndRaceDto>(e.username));
    });
}
