#include "send_vehicle_checkpoint_handler.h"

#include <iostream>

std::shared_ptr<Event> SendVehicleCheckpointHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleCheckpointDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<CheckPointCompletedEvent>(d->username, d->checkpointIndex);
}
