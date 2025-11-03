#include "sender.h"

#include <memory>
#include <vector>

#include <sys/socket.h>

#include "../../common/common_codes.h"

Sender::Sender(ServerProtocol& serverProtocol, Queue<std::shared_ptr<Dto>>& clientQueue):
        clientQueue(clientQueue), protocol(serverProtocol) {}

void Sender::run() {
    try {
        while (should_keep_running()) {
            std::shared_ptr<Dto> message = clientQueue.pop();
            protocol.sendDTO(message);
        }
    } catch (const ClosedQueue& e) {
        return;
    } catch (const SocketClosed& e) {
        return;
    }
}
Sender::~Sender() {}
