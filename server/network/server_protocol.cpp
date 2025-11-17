#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../../common/common_codes.h"
#include "../../common/serializer/serializer_map.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol(),
        serializers(SerializerMap::createSerializerMap()) {}

void ServerProtocol::sendDTO(std::shared_ptr<Dto> dto) {
    uint8_t dtoCode = dto->return_code();
    protocol.sendAction(socket, static_cast<ActionCode>(dtoCode));
    auto it = serializers.find(dtoCode);
    if (it == serializers.end() || !it->second) {
        throw std::runtime_error("Serializer not registered for dtoCode: " +
                                 std::to_string(dtoCode));
    }
    auto buffer = it->second->serialize(*dto);

    uint32_t buffer_size = buffer.size();
    protocol.sendUint32(socket, buffer_size);
    socket.sendall(buffer.data(), buffer.size());
}

std::shared_ptr<Dto> ServerProtocol::receiveDTO() {
    uint8_t dtoCode = uint8_t(protocol.receiveAction(socket));

    auto it = serializers.find(dtoCode);
    if (it == serializers.end() || !it->second) {
        throw std::runtime_error("Serializer not registered for dtoCode: " +
                                 std::to_string(dtoCode));
    }
    uint32_t buffer_size = protocol.receiveUint32(socket);
    std::vector<uint8_t> buffer(buffer_size);
    socket.recvall(buffer.data(), buffer_size);

    return it->second->deserialize(buffer);
}

ServerProtocol::~ServerProtocol() {}
