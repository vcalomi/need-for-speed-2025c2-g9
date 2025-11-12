#ifndef VEHICLE_WALL_DTO_H
#define VEHICLE_WALL_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleWallCollisionDto: public Dto {
public:
    int vehicle_id;

    VehicleWallCollisionDto(int vehicle_id);
};

#endif
