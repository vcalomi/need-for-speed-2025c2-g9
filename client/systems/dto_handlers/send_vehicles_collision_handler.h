#pragma once
#include <memory>

#include "../../common/Dto/vehicle_collision.h"
#include "../events/player_collision_event.h"

#include "dto_handler.h"

class SendVehiclesCollisionHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
