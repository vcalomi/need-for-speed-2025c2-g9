#include "player_race_finished.h"

#include <utility>

#include "../common_codes.h"

PlayerRaceFinishedDto::PlayerRaceFinishedDto(std::string username, float finishTime,
                                             int finalPosition):
        Dto(static_cast<uint8_t>(ActionCode::SEND_RACE_FINISHED)),
        username(std::move(username)),
        finishTime(finishTime),
        finalPosition(finalPosition) {};