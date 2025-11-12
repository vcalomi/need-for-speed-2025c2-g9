#ifndef VEHICLE_DTO_H
#define VEHICLE_DTO_H
#include <string>

#include "dto.h"
class VehicleDto: public Dto {
public:
    std::string username;
    float x;
    float y;
    float rotation;
    bool isAccelerating;
    bool isBraking;
    bool isAboveBridge;
    VehicleDto(const std::string& username, float x, float y, float v, bool isAccelerating = false,
               bool isBraking = false, bool isAboveBridge = false);
};

#endif
