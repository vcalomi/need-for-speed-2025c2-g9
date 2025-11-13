#pragma once
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../world/camera.h"

using SDL2pp::Renderer;

enum class ParticleType { SMOKE_ACCEL, SMOKE_BRAKE, SPARK_WALL, SPARK_VEHICLE };

struct Particle {
    float x, y;
    float vx, vy;
    float lifetime;
    float initialLifetime;
    float alpha;
    Uint8 r, g, b;
    float size;
    ParticleType type;
};

class ParticleRenderer {
public:
    explicit ParticleRenderer(Renderer& renderer);

    void Emit(float x, float y, ParticleType type, int count);
    void Update(float deltaTime);
    void Render(const Camera& camera);

private:
    Renderer& renderer_;
    std::vector<Particle> particles_;
};
