#ifndef VEHICLE_COLLISION_DTO_H
#define VEHICLE_COLLISION_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleCollisionDto: public Dto {
public:
    std::string vehicle1_username; 
    std::string vehicle2_username;

    VehicleCollisionDto(const std::string& vehicle1_username, const std::string& vehicle2_username);
};

#endif
