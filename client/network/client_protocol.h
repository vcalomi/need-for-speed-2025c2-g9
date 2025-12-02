#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../common/Dto/vehicle.h"
#include "../../common/common_codes.h"
#include "../../common/common_protocol.h"
#include "../../common/serializer/serializer.h"
#include "../../common/socket.h"

class ClientProtocol {
private:
    Socket socket;
    CommonProtocol protocol;
    std::map<uint8_t, std::unique_ptr<Serializer>> serializers;

public:
    ClientProtocol(const std::string& hostname, const std::string& port);

    std::shared_ptr<Dto> receiveDTO();
    void sendDTO(std::shared_ptr<Dto> dto);
    bool isClientConnected() const;
    void close();
    ~ClientProtocol();
};

#endif
