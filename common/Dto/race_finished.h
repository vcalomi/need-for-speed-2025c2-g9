#ifndef RACE_FINISHED_DTO_H
#define RACE_FINISHED_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class RaceFinishedDto: public Dto {
public:
    std::string username;
    float finishTime;
    int finalPosition;
    RaceFinishedDto(const std::string& username, float finishTime, int finalPosition);
};

#endif
