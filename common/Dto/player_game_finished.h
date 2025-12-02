#ifndef PLAYER_GAME_FINISHED_DTO_H
#define PLAYER_GAME_FINISHED_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class PlayerGameFinishedDto: public Dto {
public:
    std::string username;
    float totalRaceTime;
    float totalPenalties;
    int finalPosition;

    PlayerGameFinishedDto(const std::string& username, float totalRaceTime, float totalPenalties, int finalPosition);
};

#endif
