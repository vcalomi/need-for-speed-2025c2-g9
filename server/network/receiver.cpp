#include "receiver.h"

#include <memory>

#include <sys/socket.h>

#include "../../common/common_codes.h"
#include "../../common/liberror.h"

Receiver::Receiver(ServerProtocol& serverProtocol, Queue<std::shared_ptr<Dto>>& gameQueue):
        gameQueue(gameQueue), protocol(serverProtocol) {}

void Receiver::run() {
    try {
        while (should_keep_running()) {
            std::shared_ptr<Dto> dto = protocol.receiveDTO();
            gameQueue.push(dto);
        }
    } catch (const SocketClosed& e) {
        return;
    } catch (const ClosedQueue& e) {
        return;
    } catch (const LibError& e) {
        return;
    }
}

Receiver::~Receiver() {}
