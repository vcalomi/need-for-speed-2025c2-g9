#include "vehicle.h"

float PPM = 50.0f;
float SCALE_FIX = 2.66f;

Vehicle::Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id):
        world_id_(worldId), spec_(spec), spawn_(spawn), vehicle_id_(player_id) {

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
    // shapeDef.friction    = spec_.friction;
    // shapeDef.restitution = spec_.restitution;

    b2CreatePolygonShape(body_, &shapeDef, &boxShape);
}

void Vehicle::accelerate() {

    const float F = spec_.engine_force_N;
    b2Vec2 forward = b2Body_GetWorldVector(body_, b2Vec2{0.0f, 1.0f});
    b2Vec2 force = {forward.x * F, forward.y * F};

    b2Body_ApplyForceToCenter(body_, force, true);
}

void Vehicle::brake() {

    const float F = spec_.brake_force_N;
    b2Vec2 forward = b2Body_GetWorldVector(body_, b2Vec2{0.0f, -1.0f});
    b2Vec2 force = {forward.x * F, forward.y * F};

    b2Body_ApplyForceToCenter(body_, force, true);
}

void Vehicle::turn(TurnDir dir) {
    if (dir == TurnDir::None)
        return;
    b2Vec2 v = b2Body_GetLinearVelocity(body_);
    b2Vec2 fwd = b2Body_GetWorldVector(body_, b2Vec2{0.0f, 1.0f});
    float v_forward = v.x * fwd.x + v.y * fwd.y;  // m/s
    if (v_forward <= MIN_SPEED_STEER)
        return;
    float steer = (dir == TurnDir::Left) ? 1.0f : -1.0f;
    b2Body_ApplyTorque(body_, spec_.steer_torque * steer, true);
}

void Vehicle::getPosition(float& x, float& y, float& angle) const {
    b2Transform xf = b2Body_GetTransform(body_);
    x = xf.p.x * PPM * SCALE_FIX;
    y = xf.p.y * PPM * SCALE_FIX;
    angle = b2Rot_GetAngle(xf.q);
}

b2BodyId Vehicle::get_body() { return this->body_; }

void Vehicle::draw(SDL_Renderer* r, float camX_px, float camY_px, float zoom, float ppm) const {
    // 1) esquinas locales del rectángulo (en METROS)
    const float hx = width() * 0.5f;
    const float hy = height() * 0.5f;
    const b2Vec2 local[4] = {{-hx, -hy}, {+hx, -hy}, {+hx, +hy}, {-hx, +hy}};

    // 2) transformar cada esquina a MUNDO con la API de Box2D
    SDL_FPoint scr[4];
    for (int i = 0; i < 4; ++i) {
        b2Vec2 w = b2Body_GetWorldPoint(body_, local[i]);  // metros
        float px = w.x * ppm;                              // a píxeles
        float py = w.y * ppm;

        scr[i].x = (px - camX_px) * zoom;  // cámara + zoom
        scr[i].y = (py - camY_px) * zoom;
    }

    // 3) dibujar wireframe
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 180, 255, 220);
    for (int i = 0; i < 4; ++i) {
        const SDL_FPoint& A = scr[i];
        const SDL_FPoint& B = scr[(i + 1) % 4];
        SDL_RenderDrawLineF(r, A.x, A.y, B.x, B.y);
    }
}
