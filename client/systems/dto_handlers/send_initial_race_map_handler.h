#pragma once
#include <memory>

#include "../../common/Dto/initial_race_map.h"
#include "../events/race_info_event.h"

#include "dto_handler.h"

class SendInitialRaceMapHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
