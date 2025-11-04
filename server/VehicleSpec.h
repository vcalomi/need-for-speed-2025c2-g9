#pragma once
#include "constants.h"

struct VehicleSpec {
    // VehicleType name;
    float width_m;
    float height_m;
    float density;
    float engine_force_N;
    float brake_force_N;
    float steer_torque;
    float friction;
    float restitution;
};
