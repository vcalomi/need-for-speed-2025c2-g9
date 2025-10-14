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

ActionCode ServerProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

ServerProtocol::~ServerProtocol() {}
