#ifndef VEHICLE_COLLISION_DTO_H
#define VEHICLE_COLLISION_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleCollisionDto: public Dto {
public:
    std::string vehicle1_username;
    float vehicle_1_new_hp; 
    std::string vehicle2_username;
    float vehicle_2_new_hp;

    VehicleCollisionDto(const std::string& vehicle1_username, float vehicle_1_new_hp, 
                        const std::string& vehicle2_username, float vehicle_2_new_hp);
};

#endif
