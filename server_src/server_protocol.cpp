#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../common_src/common_codes.h"
#include "../common_src/serializer/vehicule_serializer.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {
    serializers[1] = VehicleSerializer();
}

// CarConfig ServerProtocol::receiveCarConfig() {
//     CarConfig car;
//     // fata implementar
//     return car;
// }

// void ServerProtocol::sendRoomList(const std::vector<std::string>& rooms) {
//     protocol.sendUint16(socket, rooms.size());
//     for (const auto& room : rooms) {
//         protocol.sendString(socket, room);
//     }
// }

void ServerProtocol::sendDTO(Dto dto) {
    uint8_t dtoCode = dto.return_code();
    auto buffer = serializers[dtoCode].serialize(dto);

    socket.sendall(buffer.data(), buffer.size());
}

Dto ServerProtocol::receiveDTO() {
    uint8_t dtoCode;
    socket.recvall(&dtoCode, sizeof(dtoCode));

    std::vector<uint8_t> buffer(serializers[dtoCode].getSize());
    socket.recvall(buffer.data(), buffer.size());

    return serializers[dtoCode].deserialize(buffer);
}

Socket ServerProtocol::releaseSocket() {
    return std::move(socket);
}

ActionCode ServerProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

ServerProtocol::~ServerProtocol() {}
