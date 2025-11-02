#include "client_receiver.h"

#include <iostream>

#include "../common/Dto/dto.h"
#include "../common/Dto/vehicle.h"
#include "../common/common_codes.h"
#include "../common/serializer/vehicle_serializer.h"

ClientReceiver::ClientReceiver(ClientProtocol& clientProtocol, Queue<std::shared_ptr<Dto>>& queue):
        queue(queue), protocol(clientProtocol) {}

void ClientReceiver::run() {
    try {
        while (should_keep_running() && !protocol.isClientConnected()) {
            std::shared_ptr<Dto> dto = protocol.receiveDTO();
            ActionCode code = static_cast<ActionCode>(dto->return_code());

            switch (code) {
                case ActionCode::SEND_CARS: {
                    auto vehicleDto = std::dynamic_pointer_cast<VehicleDto>(dto);
                    if (vehicleDto) {
                        std::cout << "Vehicle id=" << (int)vehicleDto->id << " x=" << vehicleDto->x
                                  << " y=" << vehicleDto->y << " rot=" << vehicleDto->rotation
                                  << std::endl;
                        queue.push(vehicleDto);
                    } else {
                        std::cout << "Error: Expected VehicleDto but got different type"
                                  << std::endl;
                    }
                    break;
                }
                default: {
                    std::cout << "Received DTO with code: " << (int)code << std::endl;
                    queue.push(dto);
                    break;
                }
            }
        }
    } catch (const SocketClosed& e) {
        return;
    } catch (const ClosedQueue& e) {
        return;
    }
}

ClientReceiver::~ClientReceiver() {}
