#include "client_protocol.h"

#include <stdexcept>

#include <arpa/inet.h>

#include "../../common/common_codes.h"
#include "../../common/serializer/checkpoint_serializer.h"
#include "../../common/serializer/player_move_serializer.h"
#include "../../common/serializer/player_serializer.h"
#include "../../common/serializer/players_vehicles_serializer.h"
#include "../../common/serializer/vehicle_checkpoint_serializer.h"
#include "../../common/serializer/vehicle_serializer.h"
#include "../../common/serializer/vehicle_wall_serializer.h"

// Constructor DUMMY: no se conecta a nada
ClientProtocol::ClientProtocol():
        socket(),  // no pasa host ni port
        protocol(),
        serializers() {
    // NO se conecta a nada
}

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()), protocol(), serializers() {
    serializers[static_cast<uint8_t>(ActionCode::SEND_CARS)] =
            std::make_unique<VehicleSerializer>();

    serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER)] =
            std::make_unique<PlayerSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER_MOVE)] =
            std::make_unique<PlayerMoveSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_CHECKPOINTS)] =
            std::make_unique<CheckpointSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_CHECKPOINT)] =
            std::make_unique<VehicleCheckpointSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLES_COLLISION)] =
            std::make_unique<VehicleCollisionSerializer>();
    serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_WALL_COLLISION)] =
            std::make_unique<VehicleWallCollisionSerializer>();
}

void ClientProtocol::sendUsername(const std::string& username) {
    protocol.sendAction(socket, ActionCode::SEND_USERNAME);
    protocol.sendString(socket, username);
}

void ClientProtocol::sendCreateRoom(const std::string& roomName, unsigned maxPlayers) {
    protocol.sendAction(socket, ActionCode::CREATE_ROOM);
    protocol.sendString(socket, roomName);
    protocol.sendUint16(socket, uint16_t(maxPlayers));
}

void ClientProtocol::sendJoinRoom(const std::string& roomName) {
    protocol.sendAction(socket, ActionCode::JOIN_ROOM);
    protocol.sendString(socket, roomName);
}

void ClientProtocol::sendListRooms() { protocol.sendAction(socket, ActionCode::LIST_ROOMS); }

void ClientProtocol::sendStartGame() { protocol.sendAction(socket, ActionCode::START_GAME); }

void ClientProtocol::sendChooseCar(const std::string& carType) {
    protocol.sendAction(socket, ActionCode::CHOOSE_CAR);
    protocol.sendString(socket, carType);
}

void ClientProtocol::sendListPlayers() { protocol.sendAction(socket, ActionCode::LIST_PLAYERS); }

void ClientProtocol::sendListState() { protocol.sendAction(socket, ActionCode::LIST_STATE); }

std::vector<std::string> ClientProtocol::receiveRoomList() {
    uint16_t count = protocol.receiveUint16(socket);
    std::vector<std::string> rooms;
    rooms.reserve(count);
    for (uint16_t i = 0; i < count; ++i) {
        rooms.push_back(protocol.receiveString(socket));
    }
    return rooms;
}

ActionCode ClientProtocol::receiveAction() { return protocol.receiveAction(socket); }

std::shared_ptr<Dto> ClientProtocol::receiveDTO() {
    int8_t dtoCode = uint8_t(receiveActionCode());

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

ActionCode ClientProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

bool ClientProtocol::isClientConnected() const {
    return socket.is_stream_recv_closed() || socket.is_stream_send_closed();
}

void ClientProtocol::close() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
}

ClientProtocol::~ClientProtocol() {}
