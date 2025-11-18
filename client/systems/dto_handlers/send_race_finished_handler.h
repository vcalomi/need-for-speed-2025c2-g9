#pragma once
#include <memory>

#include "../../common/Dto/race_finished.h"
#include "../events/race_finished_event.h"

#include "dto_handler.h"

class SendRaceFinishedHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
