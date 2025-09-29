#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>

#include "common_codes.h"
#include "socket.h"
#include "socket_closed.h"

class CommonProtocol {
public:
    void sendAction(Socket& Socket, ActionCode action);
    ActionCode receiveAction(Socket& socket);
    void sendMessage(Socket& socket, const std::vector<uint8_t>& msg);
    void receiveMessage(Socket& socket, std::vector<uint8_t>& msg);
    ~CommonProtocol();
};

#endif
