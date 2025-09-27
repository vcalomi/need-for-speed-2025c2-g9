#include "common_protocol.h"

#include <cstdint>
#include <vector>

#include <arpa/inet.h>

#include "common_codes.h"
#include "socket.h"

void CommonProtocol::sendAction(Socket& socket, ActionCode action) {
    uint8_t parsed = uint8_t(action);
    socket.sendall(&parsed, sizeof(parsed));
}

ActionCode CommonProtocol::receiveAction(Socket& socket) {
    uint8_t action = 0;
    socket.recvall(&action, 1);
    return ActionCode(action);
}

void CommonProtocol::sendBuffer(Socket& socket, const std::vector<uint8_t>& buffer) {
    socket.sendall(buffer.data(), buffer.size());
}

void CommonProtocol::receiveBuffer(Socket& socket, std::vector<uint8_t>& buffer) {
    socket.recvall(buffer.data(), buffer.size());
}

ActionCode CommonProtocol::tryReceiveAction(Socket& socket) {
    uint8_t action = 0;
    socket.recvsome(&action, 1);
    return ActionCode(action);
}

CommonProtocol::~CommonProtocol() {}
