#include "dto.h"

class VehicleDto : public Dto {
public:
    uint8_t id;
    // VehicleType type;
    float x;
    float y;
    float rotation;
    VehicleDto(uint8_t id, float x, float y, float v);
};
