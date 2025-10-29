#pragma once
#include <box2d/box2d.h>
#include "constants.h"
#include "VehicleSpec.h"
#include "spawn.h"
#include "constants.h"

class Vehicle {
public:
    Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id);

    void accelerate();
    void brake();
    void turn(TurnDir direction);
    void getPosition(float& x, float& y, float& angle) const;
    b2BodyId get_body();
    float width()  const noexcept { return spec_.width_m;  }
    float height() const noexcept { return spec_.height_m; }
private:
    b2WorldId   world_id_;
    VehicleSpec spec_;      
    Spawn       spawn_;     
    int         vehicle_id_;
    b2BodyId    body_;
};

