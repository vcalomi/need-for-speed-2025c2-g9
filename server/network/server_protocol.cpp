#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../../common/common_codes.h"
#include "../../common/serializer/checkpoint_serializer.h"
#include "../../common/serializer/player_move_serializer.h"
#include "../../common/serializer/player_serializer.h"
#include "../../common/serializer/vehicle_serializer.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {
    serializers[static_cast<uint8_t>(ActionCode::SEND_CARS)] =
            std::make_unique<VehicleSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER)] =
            std::make_unique<PlayerSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER_MOVE)] =
            std::make_unique<PlayerMoveSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_CHECKPOINTS)] =
            std::make_unique<CheckpointSerializer>();
}

void ServerProtocol::sendMsg(ActionCode code) { protocol.sendAction(socket, code); }

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
    uint8_t dtoCode = uint8_t(receiveActionCode());

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
