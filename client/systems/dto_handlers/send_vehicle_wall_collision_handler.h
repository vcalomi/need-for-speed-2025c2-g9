#pragma once
#include <memory>

#include "../../common/Dto/vehicle_wall_collision.h"
#include "../events/wall_collision_event.h"

#include "dto_handler.h"

class SendVehicleWallCollisionHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
