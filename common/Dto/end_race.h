#ifndef END_RACE_DTO_H
#define END_RACE_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class EndRaceDto: public Dto {
public:
    std::string username;
    EndRaceDto(const std::string& username);
};

#endif
