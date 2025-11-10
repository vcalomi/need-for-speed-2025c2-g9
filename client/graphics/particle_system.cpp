#include "./particle_system.h"

#include <cmath>
#include <cstdlib>

ParticleSystem::ParticleSystem(Renderer& renderer): renderer_(renderer) {}

void ParticleSystem::Emit(float x, float y, int count) {
    for (int i = 0; i < count; ++i) {
        float angle = static_cast<float>((rand() % 360) * M_PI / 180.0);
        float speed = static_cast<float>((rand() % 30) / 10.0f + 10.0f);

        Particle p;
        p.x = x;
        p.y = y;
        p.vx = std::cos(angle) * speed;
        p.vy = std::sin(angle) * speed;
        p.lifetime = 0.5f + static_cast<float>(rand() % 100) / 100.0f;
        p.alpha = 255.0f;
        particles_.push_back(p);
    }
}

void ParticleSystem::Update(float deltaTime) {
    for (auto& p: particles_) {
        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;
        p.lifetime -= deltaTime;
        p.alpha = std::max(0.0f, p.lifetime * 255.0f);
    }

    particles_.erase(std::remove_if(particles_.begin(), particles_.end(),
                                    [](const Particle& p) { return p.lifetime <= 0.0f; }),
                     particles_.end());
}

void ParticleSystem::Render() {
    SDL_SetRenderDrawBlendMode(renderer_.Get(), SDL_BLENDMODE_BLEND);

    for (const auto& p: particles_) {
        SDL_SetRenderDrawColor(renderer_.Get(), 255, 255, 255, static_cast<Uint8>(p.alpha));
        SDL_FRect rect = {p.x, p.y, 2, 2};
        SDL_RenderFillRectF(renderer_.Get(), &rect);
    }
}
