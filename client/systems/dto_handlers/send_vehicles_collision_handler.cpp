#include "send_vehicles_collision_handler.h"

#include <iostream>
#include <memory>

std::shared_ptr<Event> SendVehiclesCollisionHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleCollisionDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Vehicle Collision: " << d->vehicle1_username << " with "
              << d->vehicle2_username << std::endl;

    return std::make_shared<PlayerCollisionEvent>(d->vehicle1_username, d->vehicle_1_new_hp,
                                                  d->vehicle2_username, d->vehicle_2_new_hp);
}
