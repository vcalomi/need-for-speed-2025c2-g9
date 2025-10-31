#ifndef CLIENT_COMMAND
#define CLIENT_COMMAND

#include <string>
#include "common_codes.h"
#include "car_config.h"

struct ClientCommand {
    ActionCode action;
    int clientId;
    std::string roomName;
    CarConfig carConfig;
};

#endif