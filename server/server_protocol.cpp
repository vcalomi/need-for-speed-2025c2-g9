#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../common/common_codes.h"
#include "../common/serializer/vehicle_serializer.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {
    serializers[static_cast<uint8_t>(ActionCode::SEND_CARS)] =
            std::make_unique<VehicleSerializer>();
}

void ServerProtocol::sendMsg(ActionCode code) { protocol.sendAction(socket, code); }

void ServerProtocol::sendDTO(std::shared_ptr<Dto> dto) {
    uint8_t dtoCode = dto->return_code();
    protocol.sendAction(socket, static_cast<ActionCode>(dtoCode));
    auto it = serializers.find(dtoCode);
    if (it == serializers.end() || !it->second) {
        throw std::runtime_error("Serializer not registered for dtoCode: " + std::to_string(dtoCode));
    }
    auto buffer = it->second->serialize(*dto);
    socket.sendall(buffer.data(), buffer.size());
}

std::shared_ptr<Dto> ServerProtocol::receiveDTO() {
    uint8_t dtoCode = uint8_t(receiveActionCode());

    auto it = serializers.find(dtoCode);
    if (it == serializers.end() || !it->second) {
        throw std::runtime_error("Serializer not registered for dtoCode: " + std::to_string(dtoCode));
    }
    std::vector<uint8_t> buffer(it->second->getSize());
    socket.recvall(buffer.data(), buffer.size());

    return it->second->deserialize(buffer);
}

ActionCode ServerProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

std::string ServerProtocol::receiveRoomName() { return protocol.receiveString(socket); }

void ServerProtocol::sendRoomList(const std::vector<std::string>& rooms) {
    protocol.sendUint16(socket, static_cast<uint16_t>(rooms.size()));
    for (const auto& room: rooms) {
        protocol.sendString(socket, room);
    }
}

void ServerProtocol::sendErrorMsg(const std::string& /*errorMsg*/) {
    protocol.sendAction(socket, ActionCode::SEND_ERROR_MSG);
}

int ServerProtocol::receiveMaxPlayers() { return protocol.receiveUint16(socket); }

ServerProtocol::~ServerProtocol() {}
