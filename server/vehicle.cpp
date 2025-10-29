#include "vehicle.h"


Vehicle::Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id)
    : world_id_(worldId)
    , spec_(spec)
    , spawn_(spawn)
    , vehicle_id_(player_id)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {spawn_.x, spawn_.y};
    bodyDef.rotation = b2MakeRot(spawn_.angle);     
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 3.0f;

    body_ = b2CreateBody(world_id_, &bodyDef);

    b2Polygon boxShape = b2MakeBox(spec_.width_m * 0.5f, spec_.height_m * 0.5f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = spec_.density;     
    //shapeDef.friction    = spec_.friction;
    //shapeDef.restitution = spec_.restitution;

    b2CreatePolygonShape(body_, &shapeDef, &boxShape);
}

void Vehicle::accelerate() {

    const float F = spec_.engine_force_N;
    b2Vec2 forward = b2Body_GetWorldVector(body_, (b2Vec2){0.0f, 1.0f});
    b2Vec2 force = { forward.x * F, forward.y * F };

    b2Body_ApplyForceToCenter(body_, force, true);
}

void Vehicle::brake(){

    const float F = spec_.brake_force_N;
    b2Vec2 forward = b2Body_GetWorldVector(body_, (b2Vec2){0.0f, -1.0f});
    b2Vec2 force = { forward.x * F, forward.y * F };

    b2Body_ApplyForceToCenter(body_, force, true);
}

void Vehicle::turn(TurnDir dir) {
    if (dir == TurnDir::None) return;
    b2Vec2 v = b2Body_GetLinearVelocity(body_);
    b2Vec2 fwd = b2Body_GetWorldVector(body_, (b2Vec2){0.0f, 1.0f});
    float v_forward = v.x * fwd.x + v.y * fwd.y; // m/s
    if (v_forward <= MIN_SPEED_STEER) return;
    float steer = (dir == TurnDir::Left) ? 1.0f : -1.0f;
    b2Body_ApplyTorque(body_, spec_.steer_torque * steer, true);
}

void Vehicle::getPosition(float& x, float& y, float& angle) const {
    b2Transform xf = b2Body_GetTransform(body_);
    x = xf.p.x;
    y = xf.p.y;
    angle = b2Rot_GetAngle(xf.q);
}

b2BodyId Vehicle::get_body(){
    return this->body_;
}
