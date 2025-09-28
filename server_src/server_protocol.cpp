#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../common_src/common_codes.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {}

ActionCode ServerProtocol::receiveActionCode() {
    ActionCode actionCode = protocol.receiveAction(socket);
    return actionCode;
}

void ServerProtocol::sendMsg(const std::vector<uint8_t>& message) {
    protocol.sendBuffer(socket, message);
}

ActionCode ServerProtocol::tryReceiveActionCode() { return protocol.tryReceiveAction(socket); }

ServerProtocol::~ServerProtocol() {}
