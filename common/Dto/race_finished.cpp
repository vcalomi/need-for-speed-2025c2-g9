#include "race_finished.h"

#include "../common_codes.h"

RaceFinishedDto::RaceFinishedDto():
        Dto(uint8_t(ActionCode::SEND_RACE_FINISHED)) {}
