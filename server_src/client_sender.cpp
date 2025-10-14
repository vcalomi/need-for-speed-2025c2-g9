#include "client_sender.h"

#include <vector>

#include <sys/socket.h>

#include "../common_src/common_codes.h"

ClientSender::ClientSender(ServerProtocol& serverProtocol, Queue<NitroMessage>& clientQueue):
        clientQueue(clientQueue), protocol(serverProtocol) {}

void ClientSender::run() {
    try {
        while (should_keep_running()) {
            NitroMessage message = clientQueue.pop();
            protocol.sendMsg(message);
        }
    } catch (const ClosedQueue& e) {
        return;
    } catch (const SocketClosed& e) {
        return;
    }
}
ClientSender::~ClientSender() {}
