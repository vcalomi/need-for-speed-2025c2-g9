#ifndef VEHICLE_DTO_H
#define VEHICLE_DTO_H
#include <string>

#include "dto.h"

class VehicleDto: public Dto {
public:
    uint8_t id;
    float x;
    float y;
    float rotation;
    VehicleDto(uint8_t id, float x, float y, float v);
};

#endif
