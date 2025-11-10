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

void CommonProtocol::sendString(Socket& socket, const std::string& str) {
    sendUint16(socket, str.length());
    if (!str.empty()) {
        socket.sendall(str.c_str(), str.length());
    }
}

std::string CommonProtocol::receiveString(Socket& socket) {
    uint16_t length = receiveUint16(socket);
    if (length == 0) {
        return "";
    }

    std::vector<char> buffer(length);
    socket.recvall(buffer.data(), length);
    return std::string(buffer.data(), length);
}

uint32_t CommonProtocol::receiveUint32(Socket& socket) {
    uint32_t num;
    socket.recvall(&num, sizeof(num));
    return ntohl(num);
}

void CommonProtocol::sendUint32(Socket& socket, uint32_t number) {
    uint32_t parsed = htonl(number);
    socket.sendall(&parsed, sizeof(parsed));
}

CommonProtocol::~CommonProtocol() {}
