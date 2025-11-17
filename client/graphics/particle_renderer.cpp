#include "particle_renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

ParticleRenderer::ParticleRenderer(Renderer& renderer): renderer_(renderer) {}

void ParticleRenderer::Emit(float x, float y, float angle, float offset, ParticleType type,
                            int count) {

    float emitX = x + std::cos(angle) * offset;
    float emitY = y + std::sin(angle) * offset;

    for (int i = 0; i < count; ++i) {

        float randAngle = static_cast<float>((rand() % 360) * M_PI / 180.0f);

        float baseSpeed = 0.0f;
        switch (type) {
            case ParticleType::SMOKE_ACCEL:
                baseSpeed = 40.0f;
                break;
            case ParticleType::SMOKE_BRAKE:
                baseSpeed = 30.0f;
                break;
            case ParticleType::SPARK_WALL:
                baseSpeed = 120.0f;
                break;
            case ParticleType::SPARK_VEHICLE:
                baseSpeed = 150.0f;
                break;
        }

        float speed = baseSpeed + static_cast<float>(rand() % 100) / 10.0f;

        Particle p;
        p.x = emitX;
        p.y = emitY;

        p.vx = std::cos(randAngle) * speed;
        p.vy = std::sin(randAngle) * speed;

        switch (type) {
            case ParticleType::SMOKE_ACCEL:
                p.r = p.g = p.b = 200;
                p.size = 3.0f;
                p.lifetime = 0.6f;
                break;

            case ParticleType::SMOKE_BRAKE:
                p.r = p.g = p.b = 80;
                p.size = 4.0f;
                p.lifetime = 0.7f;
                break;

            case ParticleType::SPARK_WALL:
                p.r = 255;
                p.g = 200;
                p.b = 50;
                p.size = 2.0f;
                p.lifetime = 0.25f;
                break;

            case ParticleType::SPARK_VEHICLE:
                p.r = 255;
                p.g = 255;
                p.b = 255;
                p.size = 2.0f;
                p.lifetime = 0.3f;
                break;
        }

        p.initialLifetime = p.lifetime;
        p.alpha = 255.0f;
        p.type = type;

        particles_.push_back(p);
    }
}

void ParticleRenderer::Update(float deltaTime) {
    for (auto& p: particles_) {

        p.x += p.vx * deltaTime;
        p.y += p.vy * deltaTime;

        if (p.type == ParticleType::SPARK_WALL || p.type == ParticleType::SPARK_VEHICLE) {
            p.vy += 300.0f * deltaTime;
        }

        p.lifetime -= deltaTime;

        float t = std::max(p.lifetime / p.initialLifetime, 0.0f);
        p.alpha = 255.0f * (t * t);
    }

    particles_.erase(std::remove_if(particles_.begin(), particles_.end(),
                                    [](const Particle& p) { return p.lifetime <= 0.0f; }),
                     particles_.end());
}
void ParticleRenderer::Render(const Camera& camera) {
    SDL_SetRenderDrawBlendMode(renderer_.Get(), SDL_BLENDMODE_BLEND);

    for (const auto& p: particles_) {

        float screenX = p.x - camera.getX();
        float screenY = p.y - camera.getY();

        SDL_SetRenderDrawColor(renderer_.Get(), p.r, p.g, p.b, (Uint8)p.alpha);

        SDL_FRect rect = {screenX, screenY, p.size, p.size};
        SDL_RenderFillRectF(renderer_.Get(), &rect);
    }
}

void ParticleRenderer::EmitForPlayer(const World& world, const std::string& username,
                                     ParticleType type) {
    const Player& p = world.GetPlayer(username);
    float angle = p.GetAngle();

    float offset = 0.0f;

    Emit(p.GetX(), p.GetY(), angle, offset, type, 8);
}
