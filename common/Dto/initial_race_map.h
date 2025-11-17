#ifndef INITIAL_RACE_MAP_DTO_H
#define INITIAL_RACE_MAP_DTO_H
#include <string>

#include "../common_codes.h"

#include "dto.h"

class InitialRaceMapDto: public Dto {
public:
    std::string mapName;
    InitialRaceMapDto(const std::string& mapName);
};

#endif
