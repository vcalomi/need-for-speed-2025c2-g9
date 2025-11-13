#ifndef LAP_COMPLETED_DTO_H
#define LAP_COMPLETED_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class LapCompletedDto: public Dto {
public:
    std::string username;
    int current_lap;
    LapCompletedDto(const std::string& username, int current_lap);
};

#endif
