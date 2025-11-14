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
    float speed;
    bool isAboveBridge;
    VehicleDto(const std::string& username, float x, float y, float v, float speed,
               bool isAboveBridge = false);
};

#endif
