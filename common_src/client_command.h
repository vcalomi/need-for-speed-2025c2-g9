#ifndef CLIENT_COMMAND
#define CLIENT_COMMAND

#include "common_codes.h"

struct ClientCommand {
    ActionCode action;
    int clientId;
};

#endif
