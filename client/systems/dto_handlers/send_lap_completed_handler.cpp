#include "send_lap_completed_handler.h"

std::shared_ptr<Event> SendLapCompletedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<LapCompletedDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<LapCompletedEvent>(d->username, d->current_lap);
}
