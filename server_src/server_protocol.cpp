#include "server_protocol.h"

#include <cstdint>
#include <cstring>
#include <vector>

#include "../common_src/common_codes.h"

ServerProtocol::ServerProtocol(Socket& socket): socket(socket), protocol() {}

void ServerProtocol::sendMsg(const std::vector<uint8_t>& message) {
    protocol.sendMessage(socket, message);
}

ActionCode ServerProtocol::receiveActionCode() { return protocol.receiveAction(socket); }

ServerProtocol::~ServerProtocol() {}
