#ifndef VEHICLE_EXPLODED_DTO_H
#define VEHICLE_EXPLODED_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleExplodedDto: public Dto {
public:
    std::string username;

    VehicleExplodedDto(const std::string& username);
};

#endif
