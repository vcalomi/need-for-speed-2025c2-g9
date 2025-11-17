#pragma once
#include <memory>

#include "../../common/Dto/vehicle_exploded.h"
#include "../events/vehicle_exploded_event.h"

#include "dto_handler.h"

class SendVehicleExplodedHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
