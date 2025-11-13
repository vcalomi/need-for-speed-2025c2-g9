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
                events_.emplace_back(RawVehicleVehicle{A->id, B->id});
            }
        } else {
            // Vehicle + wall
            if (A && A->kind == EntityKind::Vehicle) {
                events_.emplace_back(RawVehicleWall{A->id});
            } else if (B && B->kind == EntityKind::Vehicle) {
                events_.emplace_back(RawVehicleWall{B->id});
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