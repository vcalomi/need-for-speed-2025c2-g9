#include "client_receiver.h"

#include <iostream>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/vehicle.h"
#include "../../common/common_codes.h"
#include "../../common/serializer/vehicle_serializer.h"

ClientReceiver::ClientReceiver(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& queue):
        queue(queue), protocol(clientProtocol) {}

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            std::shared_ptr<Dto> dto = protocol.receiveDTO();
            if (!should_keep_running()) break;
            queue.push(dto);
        }
    } catch (const SocketClosed& e) {
        return;
    } catch (const ClosedQueue& e) {
        return;
    } catch (const std::exception& e) {
        return;
    }
}

ClientReceiver::~ClientReceiver() {
    if (this->is_alive()) {
        this->stop();
        this->join();
    }
}
