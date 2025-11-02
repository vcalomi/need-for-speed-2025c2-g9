#ifndef CLIENT_COMMAND
#define CLIENT_COMMAND

#include <string>

#include "car_config.h"
#include "common_codes.h"

struct ClientCommand {
    ActionCode action;
    int clientId;
    std::string roomName;
    CarConfig carConfig;
};

#endif
