#ifndef VEHICLE_WALL_COLLISION_DTO_H
#define VEHICLE_WALL_COLLISION_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleWallCollisionDto: public Dto {
public:
    std::string username;

    VehicleWallCollisionDto(const std::string& username);
};

#endif
