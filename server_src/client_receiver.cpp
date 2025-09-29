#include "client_receiver.h"

#include <sys/socket.h>

#include "../common_src/common_codes.h"

ClientReceiver::ClientReceiver(ServerProtocol& serverProtocol, Queue<ClientCommand>& gameLoopQueue,
                               int clientId):
        gameLoopQueue(gameLoopQueue), protocol(serverProtocol), clientId(clientId) {}

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            ActionCode action = protocol.tryReceiveActionCode();
            ClientCommand command = {action, clientId};
            gameLoopQueue.push(command);
        }
    } catch (const SocketClosed& e) {
        // keep_running = false;
    } catch (const ClosedQueue& e) {
        // keep_running = false;
    }
}

ClientReceiver::~ClientReceiver() {}
