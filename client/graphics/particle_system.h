#pragma once
#include <vector>

#include <SDL2pp/SDL2pp.hh>

using SDL2pp::Renderer;

struct Particle {
    float x, y;
    float vx, vy;
    float lifetime;
    float alpha;
};

class ParticleSystem {
public:
    explicit ParticleSystem(Renderer& renderer);

    void Emit(float x, float y, int count);
    void Update(float deltaTime);
    void Render();

private:
    Renderer& renderer_;
    std::vector<Particle> particles_;
};
