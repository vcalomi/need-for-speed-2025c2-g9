#include "initial_race_map.h"

#include "../common_codes.h"

InitialRaceMapDto::InitialRaceMapDto(const std::string& mapName):
        Dto(uint8_t(ActionCode::SEND_INITIAL_RACE_MAP)), mapName(mapName) {}
