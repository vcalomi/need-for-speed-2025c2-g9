#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "../common_src/socket.h"
#include <string>
#include "../common_src/common_protocol.h"

class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;

public:
    explicit ServerProtocol(Socket& socket);
    ActionCode receiveActionCode();
    ActionCode tryReceiveActionCode();
    void sendMsg(const std::vector<uint8_t>& message);
    ~ServerProtocol();
};

#endif