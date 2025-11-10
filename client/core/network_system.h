#pragma once
#include "../../common/Dto/player_move.h"
#include "../../common/common_codes.h"
#include "../events/event.h"
#include "../events/player_events.h"
#include "../network/client.h"

class NetworkSystem {
public:
    explicit NetworkSystem(Client& client, EventBus& bus);

private:
    Client& client_;
};
