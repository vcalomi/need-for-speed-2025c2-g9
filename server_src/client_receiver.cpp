#include "client_receiver.h"
#include <sys/socket.h>

#include "../common_src/common_codes.h"
/*
    1. Recibe el comando 0x04 -> activar nitro
    2. Encola el comando nitro en la queue del game loop
*/

ClientReceiver::ClientReceiver(ServerProtocol& serverProtocol, Queue<ActionCode>& gameLoopQueue) :
    gameLoopQueue(gameLoopQueue), protocol(serverProtocol), keep_running(true) {}

void ClientReceiver::run() {
    try {
        while (keep_running) {
            ActionCode command = protocol.tryReceiveActionCode();
            gameLoopQueue.push(command);
        }
    } catch(const SocketClosed& e) {
        keep_running = false;
    } catch (const ClosedQueue& e) {
        keep_running = false;
    }
}

ClientReceiver::~ClientReceiver() {}
