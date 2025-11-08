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
    void sendUint16(Socket& socket, uint16_t number);
    u_int16_t receiveUint16(Socket& socket);
    void sendString(Socket& socket, const std::string& str);
    std::string receiveString(Socket& socket);
    u_int32_t receiveUint32(Socket& socket);
    void sendUint32(Socket& socket, uint32_t number);
    ~CommonProtocol();
};

#endif
