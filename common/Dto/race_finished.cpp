#include "race_finished.h"

#include "../common_codes.h"

RaceFinishedDto::RaceFinishedDto(const std::string& username):
        Dto(uint8_t(ActionCode::SEND_RACE_FINISHED)), username(username) {}
