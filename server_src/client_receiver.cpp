#include "client_receiver.h"

#include <sys/socket.h>

#include "../common_src/common_codes.h"

ClientReceiver::ClientReceiver(ServerProtocol& serverProtocol,Queue<ClientCommand>& gameQueue,
                               int clientId) :
        gameQueue(gameQueue),
        protocol(serverProtocol),
        clientId(clientId) {}

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            ActionCode action = protocol.receiveActionCode();
            ClientCommand command;
            command.action = action;
            command.clientId = clientId;
            gameQueue.push(command);
        }
    } catch (const SocketClosed& e) {
        return;
    } catch (const ClosedQueue& e) {
        return;
    }
}

ClientReceiver::~ClientReceiver() {}