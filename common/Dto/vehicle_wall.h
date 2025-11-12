#ifndef VEHICLE_WALL_DTO_H
#define VEHICLE_WALL_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleWallDto: public Dto {
public:
    int vehicle_id;

    VehicleWallDto(int vehicle_id);
};

#endif
