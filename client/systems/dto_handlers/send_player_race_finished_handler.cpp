#include "send_player_race_finished_handler.h"

#include <iostream>

std::shared_ptr<Event> SendPlayerRaceFinishedHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<PlayerRaceFinishedDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Player Race Finished: " << d->username << " time " << d->finishTime
              << " position " << d->finalPosition << std::endl;

    return std::make_shared<PlayerRaceFinishedEvent>(d->username, d->finishTime, d->finalPosition);
}
