#include "client_protocol.h"

#include <stdexcept>

#include <arpa/inet.h>

#include "../../common/common_codes.h"
#include "../../common/serializer/serializer_map.h"

// // Constructor DUMMY: no se conecta a nada
// ClientProtocol::ClientProtocol():
//         socket(),  // no pasa host ni port
//         protocol(),
//         serializers() {
//     // NO se conecta a nada
// }

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()),
        protocol(),
        serializers(SerializerMap::createSerializerMap()) {}

std::shared_ptr<Dto> ClientProtocol::receiveDTO() {
    int8_t dtoCode = uint8_t(protocol.receiveAction(socket));

    auto it = serializers.find(dtoCode);
    if (it == serializers.end()) {
        std::cerr << "Error: Unknown DTO code received: " << (int)dtoCode << std::endl;
        throw std::runtime_error("Unknown DTO code: " + std::to_string((int)dtoCode));
    }
    uint32_t buffer_size = protocol.receiveUint32(socket);
    std::vector<uint8_t> buffer(buffer_size);
    socket.recvall(buffer.data(), buffer.size());

    return it->second->deserialize(buffer);
}

void ClientProtocol::sendDTO(std::shared_ptr<Dto> dto) {
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

bool ClientProtocol::isClientConnected() const {
    return socket.is_stream_recv_closed() || socket.is_stream_send_closed();
}

void ClientProtocol::close() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

ClientProtocol::~ClientProtocol() {}
