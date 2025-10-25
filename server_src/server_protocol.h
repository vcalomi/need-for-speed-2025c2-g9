#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>

#include "../common_src/common_protocol.h"
#include "../common_src/socket.h"
#include "../common_src/car_config.h"
#include "../common_src/nitro_message.h"
class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;

public:
    explicit ServerProtocol(Socket& socket);
    ActionCode receiveActionCode();
    std::string receiveRoomName();
    CarConfig receiveCarConfig();
    void sendMsg(const NitroMessage& message);
    void sendRoomList(const std::vector<std::string>& rooms);
    
    ~ServerProtocol();
};

#endif
