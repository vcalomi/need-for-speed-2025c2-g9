#include "send_cars_handler.h"

#include <iostream>

std::shared_ptr<Event> SendCarsHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<VehicleDto>(dto);
    if (!d)
        return nullptr;

    return std::make_shared<PlayerStateUpdatedEvent>(d->username, d->x, d->y, d->rotation, d->speed,
                                                     d->isAboveBridge);
}
