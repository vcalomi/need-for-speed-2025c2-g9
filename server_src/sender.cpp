#include "sender.h"

#include <vector>

#include <sys/socket.h>

#include "../common_src/common_codes.h"

Sender::Sender(ServerProtocol& serverProtocol, Queue<NitroMessage>& clientQueue):
        clientQueue(clientQueue), protocol(serverProtocol) {}

void Sender::run() {
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
Sender::~Sender() {}
