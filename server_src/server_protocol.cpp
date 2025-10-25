#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../common_src/common_codes.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {}

void ServerProtocol::sendMsg(const NitroMessage& message) {
    protocol.sendAction(socket, message.msgCode);
    protocol.sendUint16(socket, message.carsWithNitro);
    protocol.sendAction(socket, message.nitroState);
}

CarConfig ServerProtocol::receiveCarConfig() {
    CarConfig car;
    // fata implementar
    return car;
}

void ServerProtocol::sendRoomList(const std::vector<std::string>& rooms) {
    protocol.sendUint16(socket, rooms.size());
    for (const auto& room : rooms) {
        protocol.sendString(socket, room);
    }
}

std::string ServerProtocol::receiveRoomName() {
    return protocol.receiveString(socket);
}

ActionCode ServerProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

ServerProtocol::~ServerProtocol() {}
