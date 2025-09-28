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

void CommonProtocol::sendBuffer(Socket& socket, const std::vector<uint8_t>& message) {
    int sent_bytes = socket.sendall(message.data(), message.size());
    if (sent_bytes <= 0) {
        throw SocketClosed();
    }
}

void CommonProtocol::receiveBuffer(Socket& socket, std::vector<uint8_t>& buffer) {
    socket.recvall(buffer.data(), buffer.size());
}

ActionCode CommonProtocol::tryReceiveAction(Socket& socket) {
    uint8_t action = 0;
    int received = socket.recvsome(&action, 1);
    if (received <= 0) {
        throw SocketClosed();
    }
    
    return ActionCode(action);
}

CommonProtocol::~CommonProtocol() {}
