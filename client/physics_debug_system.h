#pragma once
#include <SDL2pp/SDL2pp.hh>
#include <box2d/box2d.h>

class PhysicsDebugSystem {
public:
    explicit PhysicsDebugSystem(SDL2pp::Renderer& renderer);
    void RenderWorld(b2WorldId worldId, float pixelsPerMeter = 50.0f);

private:
    SDL2pp::Renderer& renderer_;

    void DrawBody(b2BodyId bodyId, float ppm);
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, float ppm);
};
