#include "send_vehicle_wall_collision_handler.h"

#include <iostream>
#include <memory>

std::shared_ptr<Event> SendVehicleWallCollisionHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleWallCollisionDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Vehicle Wall Collision: " << d->username << std::endl;

    return std::make_shared<WallCollisionEvent>(d->username, d->vehicle_new_hp);
}
