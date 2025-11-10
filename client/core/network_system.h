#pragma once
#include "../../common/Dto/player_move.h"
#include "../../common/common_codes.h"
#include "../events/event.h"
#include "../events/player_events.h"
#include "../network/client.h"

class NetworkSystem {
public:
    NetworkSystem(Client& client, EventBus& eventBus);

private:
    Client& client_;
    EventBus& eventBus_;
};
