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
    ActionCode tryReceiveAction(Socket& socket);
    ActionCode receiveAction(Socket& socket);
    void sendBuffer(Socket& socket, const std::vector<uint8_t>& buffer);
    void receiveBuffer(Socket& socket, std::vector<uint8_t>& buffer);
    ~CommonProtocol();
};

#endif