#include "send_vehicle_exploded_handler.h"

std::shared_ptr<Event> SendVehicleExplodedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleExplodedDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<VehicleExplodedEvent>(d->username);
}
