#include "send_race_finished_handler.h"

#include <iostream>
#include <memory>

std::shared_ptr<Event> SendRaceFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<RaceFinishedDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Race Finished" << std::endl;

    return std::make_shared<RaceFinishedEvent>();
}
