#include "client_protocol.h"

#include "../common/common_codes.h"
#include "../common/serializer/vehicle_serializer.h"
#include <arpa/inet.h>
#include <stdexcept>

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()), protocol(), serializers() {
    serializers[static_cast<uint8_t>(ActionCode::SEND_CARS)] = std::make_unique<VehicleSerializer>();
}

void ClientProtocol::sendCreateRoom(const std::string& roomName) {
    protocol.sendAction(socket, ActionCode::CREATE_ROOM);
    protocol.sendString(socket, roomName);
}

void ClientProtocol::sendJoinRoom(const std::string& roomName) {
    protocol.sendAction(socket, ActionCode::JOIN_ROOM);
    protocol.sendString(socket, roomName);
}

void ClientProtocol::sendListRooms() {
    protocol.sendAction(socket, ActionCode::LIST_ROOMS);
}

void ClientProtocol::sendStartGame() {
    protocol.sendAction(socket, ActionCode::START_GAME);
}

void ClientProtocol::sendChooseCar(const std::string& carType) {
    protocol.sendAction(socket, ActionCode::CHOOSE_CAR);
    // protocol.sendString(socket, carType); // Implementar cuando haya CarConfig
}

std::vector<std::string> ClientProtocol::receiveRoomList() {
    uint16_t count = protocol.receiveUint16(socket);
    std::vector<std::string> rooms;
    rooms.reserve(count);
    for (uint16_t i = 0; i < count; ++i) {
        rooms.push_back(protocol.receiveString(socket));
    }
    return rooms;
}

ActionCode ClientProtocol::receiveAction() {
    return protocol.receiveAction(socket);
}

std::shared_ptr<Dto> ClientProtocol::receiveDTO() {
    int8_t dtoCode = uint8_t(receiveActionCode());

    auto it = serializers.find(dtoCode);
    if (it == serializers.end()) {
        std::cerr << "Error: Unknown DTO code received: " << (int)dtoCode << std::endl;
        throw std::runtime_error("Unknown DTO code: " + std::to_string((int)dtoCode));
    }

    std::vector<uint8_t> buffer(it->second->getSize());
    socket.recvall(buffer.data(), buffer.size());

    return it->second->deserialize(buffer);
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
