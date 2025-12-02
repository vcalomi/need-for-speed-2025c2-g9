#include "send_race_finished_handler.h"

#include <memory>

std::shared_ptr<Event> SendRaceFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<RaceFinishedDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<RaceFinishedEvent>();
}
