#include "client_protocol.h"
#include "../common_src/common_codes.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
    socket(hostname.c_str(), port.c_str()), protocol() {}

void ClientProtocol::sendNitro() {
    protocol.sendAction(socket, ActionCode::ACTIVATE_NITRO);
}

std::vector<uint8_t> ClientProtocol::receiveMessage() {
    std::vector<uint8_t> message(4);
    protocol.receiveBuffer(socket, message);
    return message;
}

ClientProtocol::~ClientProtocol() {}
