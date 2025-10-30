#include "sender.h"

#include <vector>

#include <sys/socket.h>

#include "../common_src/common_codes.h"

Sender::Sender(ServerProtocol& serverProtocol, Queue<Dto>& clientQueue):
        clientQueue(clientQueue), protocol(serverProtocol) {}

void Sender::run() {
    try {
        while (should_keep_running()) {
            Dto message = clientQueue.pop();
            protocol.sendDTO(message);
        }
    } catch (const ClosedQueue& e) {
        return;
    } catch (const SocketClosed& e) {
        return;
    }
}
Sender::~Sender() {}
