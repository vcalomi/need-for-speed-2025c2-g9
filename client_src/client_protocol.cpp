#include "client_protocol.h"

#include "../common_src/common_codes.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()), protocol() {}

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

NitroMessage ClientProtocol::receiveMessage() {
    NitroMessage msg;
    msg.msgCode = protocol.receiveAction(socket);
    msg.carsWithNitro = protocol.receiveUint16(socket);
    msg.nitroState = protocol.receiveAction(socket);
    return msg;
}

ClientProtocol::~ClientProtocol() {}
