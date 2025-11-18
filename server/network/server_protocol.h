#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../common/Dto/dto.h"
#include "../../common/car_config.h"
#include "../../common/common_protocol.h"
#include "../../common/serializer/serializer.h"
#include "../../common/socket.h"

class ServerProtocol {
private:
    Socket& socket;
    CommonProtocol protocol;
    std::map<uint8_t, std::unique_ptr<Serializer>> serializers;

public:
    explicit ServerProtocol(Socket& socket);
    void sendDTO(std::shared_ptr<Dto> dto);
    std::shared_ptr<Dto> receiveDTO();
    ~ServerProtocol();
};

#endif
