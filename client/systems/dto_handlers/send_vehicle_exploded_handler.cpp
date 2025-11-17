#include "send_vehicle_exploded_handler.h"

#include <iostream>

std::shared_ptr<Event> SendVehicleExplodedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleExplodedDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Vehicle Exploded: " << d->username << std::endl;

    return std::make_shared<VehicleExplodedEvent>(d->username);
}
