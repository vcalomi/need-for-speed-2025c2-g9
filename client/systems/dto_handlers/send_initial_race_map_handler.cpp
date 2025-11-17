#include "send_initial_race_map_handler.h"

#include <iostream>
#include <memory>

std::shared_ptr<Event> SendInitialRaceMapHandler::Handle(const std::shared_ptr<Dto>& dto) {
    auto d = std::dynamic_pointer_cast<InitialRaceMapDto>(dto);
    if (!d)
        return nullptr;

    std::cout << "[DtoHandler] Initial Race Map received" << std::endl;

    return std::make_shared<RaceInfoEvent>(d->mapName);
}
