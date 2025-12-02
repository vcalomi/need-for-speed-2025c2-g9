#include "player_game_finished.h"

#include "../common_codes.h"

PlayerGameFinishedDto::PlayerGameFinishedDto(const std::string& username, float totalRaceTime, float totalPenalties, int finalPosition):
        Dto(uint8_t(ActionCode::SEND_PLAYER_GAME_FINISHED)), 
        username(username),
        totalRaceTime(totalRaceTime),
        totalPenalties(totalPenalties),
        finalPosition(finalPosition) {}
