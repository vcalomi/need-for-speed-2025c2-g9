#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>

#include "../common_src/common_protocol.h"
#include "../common_src/nitro_message.h"
#include "../common_src/socket.h"

class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;

public:
    explicit ServerProtocol(Socket& socket);
    ActionCode receiveActionCode();
    void sendMsg(const NitroMessage& message);
    ~ServerProtocol();
};

#endif
