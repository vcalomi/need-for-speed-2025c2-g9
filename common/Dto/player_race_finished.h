#ifndef PLAYER_RACE_FINISHED_DTO_H
#define PLAYER_RACE_FINISHED_DTO_H
#include <string>

#include "dto.h"

class PlayerRaceFinishedDto: public Dto {
public:
    std::string username;
    float finishTime;
    int finalPosition;
    PlayerRaceFinishedDto(std::string username, float finishTime, int finalPosition);
};

#endif
