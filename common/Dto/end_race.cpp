#include "end_race.h"

#include "../common_codes.h"

EndRaceDto::EndRaceDto():
        Dto(uint8_t(ActionCode::SEND_END_RACE)) {}
