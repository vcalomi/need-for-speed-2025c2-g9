#include "./renderer_system.h"

RendererSystem::RendererSystem(SDL2pp::Renderer& renderer, SpriteSheet& cars):
        renderer_(renderer), cars_(cars) {}

void RendererSystem::Render(const World& world) {
    renderer_.Clear();

    for (const auto& [id, player]: world.GetPlayers()) {
        DrawPlayer(player);
    }

    renderer_.Present();
}

void RendererSystem::DrawPlayer(const Player& player) {
    std::string spriteName = player.GetSpriteForAngle(player.GetAngle());
    const Rect& src = cars_.GetSprite(spriteName);
    Rect dest(player.GetX() - src.GetW() / 2, player.GetY() - src.GetH() / 2, src.GetW(),
              src.GetH());
    renderer_.Copy(cars_.GetTexture(), src, dest);
}
