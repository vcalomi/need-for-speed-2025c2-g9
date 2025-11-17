#include "send_lap_completed_handler.h"

#include <iostream>

std::shared_ptr<Event> SendLapCompletedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<LapCompletedDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Lap Completed: " << d->username << " current lap " << d->current_lap
              << std::endl;

    return std::make_shared<LapCompletedEvent>(d->username, d->current_lap);
}
