#pragma once
#include <memory>

#include "../../common/Dto/vehicle_checkpoint.h"
#include "../events/checkpoint_completed_event.h"

#include "dto_handler.h"

class SendVehicleCheckpointHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
