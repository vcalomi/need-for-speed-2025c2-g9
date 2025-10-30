#include "vehicle.h"

VehicleDto::VehicleDto(uint8_t id, float x, float y, float v) 
        : id(id), x(x), y(y), rotation(v) {}