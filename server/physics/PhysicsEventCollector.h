#pragma once
#include <vector>
#include <variant>
#include <box2d/box2d.h>
#include "EntityTags.h"

struct RawVehicleCheckpoint { int vehicleId; int checkpointIndex; };
struct RawVehicleVehicle   { int a, b; };
struct RawVehicleWall      { int vehicleId; };


using RawEvent = std::variant<RawVehicleCheckpoint, RawVehicleVehicle, RawVehicleWall>;

class PhysicsEventCollector {

public:
    PhysicsEventCollector() = default;

    void collect(b2WorldId world);
    std::vector<RawEvent> drain();

private:

    std::vector<RawEvent> events_;

};