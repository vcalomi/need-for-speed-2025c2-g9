#include "client_sender.h"

#include <vector>

#include <sys/socket.h>

#include "../common/common_codes.h"

ClientSender::ClientSender(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& clientQueue):
        clientQueue(clientQueue), protocol(clientProtocol) {}

void ClientSender::run() {
    try {
        while (should_keep_running()) {
            std::shared_ptr<Dto> message = clientQueue.pop();
            protocol.sendDTO(message);
        }
    } catch (const ClosedQueue& e) {
        return;
    } catch (const SocketClosed& e) {
        return;
    } catch (const std::exception& e) {
        std::cerr << "ClientSender error: " << e.what() << std::endl;
        return;
    }
}
ClientSender::~ClientSender() {}
