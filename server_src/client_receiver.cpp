#include "client_receiver.h"
#include <sys/socket.h>

#include "../common_src/common_codes.h"
/*
    1. Recibe el comando 0x04 -> activar nitro
    2. Encola el comando nitro en la queue del game loop
*/

ClientReceiver::ClientReceiver(Socket& socket, Queue<ActionCode>& gameLoopQueue) :
    peer(socket), gameLoopQueue(gameLoopQueue), protocol(socket), keep_running(true) { start(); }

void ClientReceiver::run() {
    while (keep_running) {
        ActionCode command = protocol.receiveActionCode();
        gameLoopQueue.try_push(command);
    }
}

ClientReceiver::~ClientReceiver() {}
