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
struct RawVehicleBridgeToggle { int vehicleId; bool under; };
struct RawVehicleNpc {int vehicleId; int npcId; };


using RawEvent = std::variant<RawVehicleCheckpoint, RawVehicleVehicle, RawVehicleWall, RawVehicleBridgeToggle, RawVehicleNpc>;

class PhysicsEventCollector {

public:
    PhysicsEventCollector() = default;
    void capturePreStepSpeeds(const std::unordered_map<int, std::unique_ptr<Vehicle>>& vehicles);
    void collect(b2WorldId world);
    std::vector<RawEvent> drain();
    const std::unordered_map<int, float>& getLastSpeeds() const { return lastSpeeds_;}

private:
    std::unordered_map<int, float> lastSpeeds_;
    std::vector<RawEvent> events_;
    float getSpeedFor(int vehicleId) const;
};