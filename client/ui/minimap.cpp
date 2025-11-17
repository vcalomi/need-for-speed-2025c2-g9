#include "minimap.h"

#include <algorithm>

constexpr float MINIMAP_ZOOM_FACTOR = 3.0f;

constexpr int MINIMAP_MARGIN_X = 10;
constexpr int MINIMAP_MARGIN_Y = 10;

constexpr int PLAYER_DOT_SIZE = 4;
constexpr int PLAYER_DOT_HALF = PLAYER_DOT_SIZE / 2;

constexpr SDL_Color LOCAL_PLAYER_COLOR = {0, 255, 0, 255};
constexpr SDL_Color OTHER_PLAYER_COLOR = {255, 0, 0, 255};
constexpr SDL_Color CAMERA_RECT_COLOR = {255, 255, 255, 120};

constexpr float HALF_DIVISOR = 2.0f;

Minimap::Minimap(SDL2pp::Renderer& renderer, const Map& map, int width, int height):
        renderer_(renderer),
        mapTexture_(const_cast<SDL2pp::Texture*>(map.GetBackgroundTexture())),
        miniWidth(width),
        miniHeight(height) {}


void Minimap::Render(const World& world, const Camera& camera) {
    if (!mapTexture_)
        return;


    const float viewW = camera.GetViewportWidth() * MINIMAP_ZOOM_FACTOR;
    const float viewH = camera.GetViewportHeight() * MINIMAP_ZOOM_FACTOR;

    float srcX = camera.getX() + camera.GetViewportWidth() / HALF_DIVISOR - viewW / HALF_DIVISOR;
    float srcY = camera.getY() + camera.GetViewportHeight() / HALF_DIVISOR - viewH / HALF_DIVISOR;

    srcX = std::clamp(srcX, 0.0f, (float)(mapTexture_->GetWidth() - viewW));
    srcY = std::clamp(srcY, 0.0f, (float)(mapTexture_->GetHeight() - viewH));

    SDL2pp::Rect src((int)srcX, (int)srcY, (int)viewW, (int)viewH);
    SDL2pp::Rect dst(MINIMAP_MARGIN_X, MINIMAP_MARGIN_Y, miniWidth, miniHeight);

    renderer_.Copy(*mapTexture_, src, dst);

    float scaleX = miniWidth / viewW;
    float scaleY = miniHeight / viewH;

    // Render de jugadores
    for (const auto& [id, player]: world.GetPlayers()) {
        float relX = (player.GetX() - srcX) * scaleX;
        float relY = (player.GetY() - srcY) * scaleY;

        int dotX = MINIMAP_MARGIN_X + (int)relX;
        int dotY = MINIMAP_MARGIN_Y + (int)relY;

        const SDL_Color& color = (player.GetUsername() == world.GetLocalPlayer().GetUsername()) ?
                                         LOCAL_PLAYER_COLOR :
                                         OTHER_PLAYER_COLOR;

        renderer_.SetDrawColor(color.r, color.g, color.b, color.a);
        renderer_.FillRect(SDL2pp::Rect(dotX - PLAYER_DOT_HALF, dotY - PLAYER_DOT_HALF,
                                        PLAYER_DOT_SIZE, PLAYER_DOT_SIZE));
    }

    // Checkpoints
    if (!world.GetCheckpoints().empty() && world.HasPlayers()) {
        const auto& localPlayer = world.GetLocalPlayer();
        const auto& activeCp = world.GetActiveCheckpointFor(localPlayer.GetUsername());
        const auto passed = world.GetPassedCheckpointIdsFor(localPlayer.GetUsername());

        for (const auto& cp: world.GetCheckpoints()) {
            float relX = (cp.x - srcX) * scaleX;
            float relY = (cp.y - srcY) * scaleY;

            int dotX = MINIMAP_MARGIN_X + (int)relX;
            int dotY = MINIMAP_MARGIN_Y + (int)relY;

            SDL_Color color;

            if (cp.id == activeCp.id)
                color = {255, 255, 255, 255};
            else if (passed.count(cp.id))
                color = {0, 255, 0, 180};
            else
                color = {180, 180, 180, 150};

            renderer_.SetDrawColor(color.r, color.g, color.b, color.a);
            renderer_.FillRect(SDL2pp::Rect(dotX - 2, dotY - 2, 4, 4));
        }
    }

    // Rectángulo de cámara
    renderer_.SetDrawColor(CAMERA_RECT_COLOR.r, CAMERA_RECT_COLOR.g, CAMERA_RECT_COLOR.b,
                           CAMERA_RECT_COLOR.a);

    SDL2pp::Rect camRect(MINIMAP_MARGIN_X + (int)((camera.getX() - srcX) * scaleX),
                         MINIMAP_MARGIN_Y + (int)((camera.getY() - srcY) * scaleY),
                         (int)(camera.GetViewportWidth() * scaleX),
                         (int)(camera.GetViewportHeight() * scaleY));

    renderer_.DrawRect(camRect);
}
