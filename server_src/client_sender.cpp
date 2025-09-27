#include "client_sender.h"
#include <sys/socket.h>

#include "../common_src/common_codes.h"

ClientSender::ClientSender(Socket& socket) :
    peer(socket), clientQueue(), protocol(socket), keep_running(true) {}

void ClientSender::run() {
    while (keep_running) {
        std::vector<uint8_t> message = clientQueue.pop();
        protocol.sendMsg(message);
    }
}

void ClientSender::stop() {
    keep_running = false;
    clientQueue.close();
}

ClientSender::~ClientSender() {}
