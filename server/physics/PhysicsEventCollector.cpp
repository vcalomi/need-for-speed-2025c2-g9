#include "PhysicsEventCollector.h"
#include <iostream>

std::vector<RawEvent> PhysicsEventCollector::drain() {
    auto out = std::move(events_); 
    events_.clear();               
    return out;
}

void PhysicsEventCollector::collect(b2WorldId world)
{

    b2ContactEvents ce = b2World_GetContactEvents(world);

    for (int i = 0; i < ce.beginCount; ++i) {
        const b2ContactBeginTouchEvent& e = ce.beginEvents[i];

        auto* A = static_cast<FixtureTag*>(b2Shape_GetUserData(e.shapeIdA));
        auto* B = static_cast<FixtureTag*>(b2Shape_GetUserData(e.shapeIdB));

        if (A && B) {
            // Vehicle + Vehicle
            if (A->kind == EntityKind::Vehicle && B->kind == EntityKind::Vehicle) {
                float speedA = getSpeedFor(A->id);
                float speedB = getSpeedFor(B->id);
                events_.emplace_back(RawVehicleVehicle{A->id, B->id, speedA, speedB});
            }
        } else {
            // Vehicle + wall
            if (A && A->kind == EntityKind::Vehicle) {
                float speed = getSpeedFor(A->id);
                events_.emplace_back(RawVehicleWall{A->id, speed});
            } else if (B && B->kind == EntityKind::Vehicle) {
                float speed = getSpeedFor(B->id);
                events_.emplace_back(RawVehicleWall{B->id, speed});
            }
        }
    }

    b2SensorEvents se = b2World_GetSensorEvents(world);

    for (int i = 0; i < se.beginCount; ++i) {
        const b2SensorBeginTouchEvent& e = se.beginEvents[i];

        auto* sensorTag  = static_cast<FixtureTag*>(b2Shape_GetUserData(e.sensorShapeId));
        auto* visitorTag = static_cast<FixtureTag*>(b2Shape_GetUserData(e.visitorShapeId));
        
        if (sensorTag->kind == EntityKind::Checkpoint && visitorTag->kind == EntityKind::Vehicle) {
            events_.emplace_back(RawVehicleCheckpoint{visitorTag->id, sensorTag->id});
        }
    }
}

void PhysicsEventCollector::capturePreStepSpeeds(const std::unordered_map<int, std::unique_ptr<Vehicle>>& vehicles){
    lastSpeeds_.clear();

    for (auto& [id, v] : vehicles) {
        b2Vec2 vel = b2Body_GetLinearVelocity(v->get_body());
        float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        lastSpeeds_[id] = speed; 
    }
}

float PhysicsEventCollector::getSpeedFor(int vehicleId) const {
    auto it = lastSpeeds_.find(vehicleId);
    if (it != lastSpeeds_.end())
        return it->second;
    return 0.0f;
}