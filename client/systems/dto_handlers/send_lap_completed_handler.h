#pragma once
#include <memory>

#include "../../common/Dto/lap_completed.h"
#include "../events/lap_completed_event.h"

#include "dto_handler.h"

class SendLapCompletedHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
