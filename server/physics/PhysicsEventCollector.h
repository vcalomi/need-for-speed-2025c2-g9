#pragma once
#include <vector>
#include <variant>
#include <box2d/box2d.h>
#include "EntityTags.h"
#include <unordered_map>
#include <memory>
#include "vehicle.h"
struct RawVehicleCheckpoint { int vehicleId; int checkpointIndex; };
struct RawVehicleVehicle   { int a; int b; float speedA; float speedB; };
struct RawVehicleWall      { int vehicleId; float speedBeforeImpact; };


using RawEvent = std::variant<RawVehicleCheckpoint, RawVehicleVehicle, RawVehicleWall>;

class PhysicsEventCollector {

public:
    PhysicsEventCollector() = default;
    void capturePreStepSpeeds(const std::unordered_map<int, std::unique_ptr<Vehicle>>& vehicles);
    void collect(b2WorldId world);
    std::vector<RawEvent> drain();

private:
    std::unordered_map<int, float> lastSpeeds_;
    std::vector<RawEvent> events_;
    float getSpeedFor(int vehicleId) const;
};