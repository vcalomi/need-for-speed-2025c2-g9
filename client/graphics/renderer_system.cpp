#include "./renderer_system.h"

#define HALF_DIVISOR 2.0f

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars):
        renderer_(renderer), cars_(cars) {}

void RendererSystem::Render(const World& world, Map& map, const Camera& camera) {
    renderer_.Clear();
    map.Render(renderer_, camera);
    for (const auto& [id, player]: world.GetPlayers()) DrawPlayer(player, camera);
    renderer_.Present();
}

void RendererSystem::DrawPlayer(const Player& player, const Camera& camera) {
    std::string spriteName = player.GetSpriteForAngle(player.GetAngle());
    if (!cars_.HasSprite(spriteName)) {
        std::cerr << "[Renderer] Sprite '" << spriteName << "' not found, skipping draw\n";
        return;
    }

    const Rect& src = cars_.GetSprite(spriteName);

    float drawX = player.GetX() - camera.getX() - src.GetW() / HALF_DIVISOR;
    float drawY = player.GetY() - camera.getY() - src.GetH() / HALF_DIVISOR;

    Rect dest(drawX, drawY, src.GetW(), src.GetH());
    renderer_.Copy(cars_.GetTexture(), src, dest);
}
