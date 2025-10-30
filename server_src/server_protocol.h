#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>
#include <map>

#include "../common_src/common_protocol.h"
#include "../common_src/socket.h"
#include "../common_src/car_config.h"
#include "../common_src/Dto/dto.h"
#include "../common_src/serializer/serializer.h"

class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;
    std::map<uint8_t, Serializer> serializers;

public:
    explicit ServerProtocol(Socket& socket);
    void sendDTO(Dto dto);
    Dto receiveDTO();
    ActionCode receiveActionCode();
    Socket releaseSocket();
    std::string receiveRoomName();
    CarConfig receiveCarConfig();
    void sendActionCode();
    void sendRoomList(const std::vector<std::string>& rooms);
    void sendErrorMsg(const std::string& errorMsg);
    
    ~ServerProtocol();
};

#endif
