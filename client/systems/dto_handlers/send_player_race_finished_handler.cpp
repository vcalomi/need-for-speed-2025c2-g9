#include "send_player_race_finished_handler.h"

std::shared_ptr<Event> SendPlayerRaceFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<PlayerRaceFinishedDto>(dto);
    if (!d)
        return nullptr;
    return std::make_shared<PlayerRaceFinishedEvent>(d->username, d->finishTime, d->finalPosition);
}
