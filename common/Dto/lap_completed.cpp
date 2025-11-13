#include "lap_completed.h"

#include "../common_codes.h"

LapCompletedDto::LapCompletedDto(const std::string& username, int  current_lap):
        Dto(uint8_t(ActionCode::SEND_LAP_COMPLETED)), username(username), current_lap(current_lap) {}
