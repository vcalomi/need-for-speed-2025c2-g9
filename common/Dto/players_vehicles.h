#ifndef PLAYERS_VEHICLES_DTO_H
#define PLAYERS_VEHICLES_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleCollisionDto: public Dto {
public:
    int vehicle1_id;
    int vehicle2_id;

    VehicleCollisionDto(int vehicle1_id, int vehicle2_id);
};

#endif
