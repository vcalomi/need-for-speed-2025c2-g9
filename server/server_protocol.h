#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "../common/common_protocol.h"
#include "../common/socket.h"
#include "../common/car_config.h"
#include "../common/Dto/dto.h"
#include "../common/serializer/serializer.h"

class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;
    std::map<uint8_t, std::unique_ptr<Serializer>> serializers;

public:
    explicit ServerProtocol(Socket& socket);
    void sendDTO(std::shared_ptr<Dto> dto);
    std::shared_ptr<Dto> receiveDTO();
    ActionCode receiveActionCode();
    std::string receiveRoomName();
    int receiveMaxPlayers();
    void sendActionCode();
    void sendRoomList(const std::vector<std::string>& rooms);
    void sendErrorMsg(const std::string& errorMsg);
    void sendMsg(ActionCode code);
    ~ServerProtocol();
};

#endif
