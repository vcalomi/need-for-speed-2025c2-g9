#include "client_protocol.h"

#include "../common_src/common_codes.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()), protocol() {}

void ClientProtocol::sendNitro() { protocol.sendAction(socket, ActionCode::ACTIVATE_NITRO); }

NitroMessage ClientProtocol::receiveMessage() {
    NitroMessage msg;
    msg.msgCode = protocol.receiveAction(socket);
    msg.carsWithNitro = protocol.receiveUint16(socket);
    msg.nitroState = protocol.receiveAction(socket);
    return msg;
}

ClientProtocol::~ClientProtocol() {}
