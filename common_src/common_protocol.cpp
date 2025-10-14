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

void CommonProtocol::sendUint16(Socket& socket, uint16_t number) {
    uint16_t parsed = htons(number);
    socket.sendall(&parsed, sizeof(parsed));
}

u_int16_t CommonProtocol::receiveUint16(Socket& socket) {
    uint16_t num;
    socket.recvall(&num, sizeof(num));
    return ntohs(num);
}

ActionCode CommonProtocol::receiveAction(Socket& socket) {
    uint8_t action = 0;
    int received = socket.recvsome(&action, 1);
    if (received <= 0) {
        throw SocketClosed();
    }

    return ActionCode(action);
}

CommonProtocol::~CommonProtocol() {}
