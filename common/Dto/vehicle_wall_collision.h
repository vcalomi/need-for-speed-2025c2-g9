#ifndef VEHICLE_WALL_COLLISION_DTO_H
#define VEHICLE_WALL_COLLISION_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleWallCollisionDto: public Dto {
public:
    std::string username;
    int vehicle_new_hp;
    VehicleWallCollisionDto(const std::string& username, int vehicle_new_hp);
};

#endif
