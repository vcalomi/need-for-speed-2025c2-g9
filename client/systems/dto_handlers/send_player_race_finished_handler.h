#pragma once
#include <memory>

#include "../../common/Dto/player_race_finished.h"
#include "../events/player_race_finished_event.h"

#include "dto_handler.h"

class SendPlayerRaceFinishedHandler: public DtoHandler {
public:
    std::shared_ptr<Event> Handle(const std::shared_ptr<Dto>& dto) override;
};
