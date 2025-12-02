#pragma once
#include <memory>

#include "../../common/Dto/vehicle.h"
#include "../events/player_events.h"

#include "dto_handler.h"

class SendNpcHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
