#include "./renderer_system.h"

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
    std::cout <<  camera.x;
    if (!cars_.HasSprite(spriteName)) {
        std::cerr << "[Renderer] Sprite '" << spriteName << "' not found, skipping draw\n";
        return;
    }

    const Rect& src = cars_.GetSprite(spriteName);

    // Convertir coordenadas del jugador al sistema visible (restando cámara)
    float drawX = player.GetX();
    float drawY = player.GetY();

    Rect dest(drawX, drawY, src.GetW(), src.GetH());
    renderer_.Copy(cars_.GetTexture(), src, dest);
}
