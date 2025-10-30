#include "receiver.h"

#include <sys/socket.h>

#include "../common_src/common_codes.h"

Receiver::Receiver(ServerProtocol& serverProtocol,Queue<Dto>& gameQueue) :
        gameQueue(gameQueue),
        protocol(serverProtocol) {}

void Receiver::run() {
    try {
        while (should_keep_running()) {
            Dto dto = protocol.receiveDTO();
            gameQueue.push(dto);
        }
    } catch (const SocketClosed& e) {
        return;
    } catch (const ClosedQueue& e) {
        return;
    }
}

Receiver::~Receiver() {}