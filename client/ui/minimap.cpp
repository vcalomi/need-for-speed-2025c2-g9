#include "minimap.h"

#include <algorithm>

// Zoom factor for the visible area relative to the camera (3x more area)
constexpr float MINIMAP_ZOOM_FACTOR = 3.0f;

// Minimap position on screen
constexpr int MINIMAP_MARGIN_X = 10;
constexpr int MINIMAP_MARGIN_Y = 10;

// Player dot size
constexpr int PLAYER_DOT_SIZE = 4;
constexpr int PLAYER_DOT_HALF = PLAYER_DOT_SIZE / 2;

// Local player color (green)
constexpr SDL_Color LOCAL_PLAYER_COLOR = {0, 255, 0, 255};

// Color of other players (red)
constexpr SDL_Color OTHER_PLAYER_COLOR = {255, 0, 0, 255};

// Color and opacity of the camera rectangle
constexpr SDL_Color CAMERA_RECT_COLOR = {255, 255, 255, 120};

constexpr float HALF_DIVISOR = 2.0f;

Minimap::Minimap(SDL2pp::Renderer& renderer, const Map& map, int width, int height):
        renderer_(renderer), mapTexture_(map.GetTexture()), miniWidth(width), miniHeight(height) {}

void Minimap::Render(const World& world, const Camera& camera) {
    const float viewW = camera.GetViewportWidth() * MINIMAP_ZOOM_FACTOR;
    const float viewH = camera.GetViewportHeight() * MINIMAP_ZOOM_FACTOR;

    float srcX = camera.getX() + camera.GetViewportWidth() / HALF_DIVISOR - viewW / HALF_DIVISOR;
    float srcY = camera.getY() + camera.GetViewportHeight() / HALF_DIVISOR - viewH / HALF_DIVISOR;
    srcX = std::max(0.0f, std::min(srcX, static_cast<float>(mapTexture_.GetWidth() - viewW)));
    srcY = std::max(0.0f, std::min(srcY, static_cast<float>(mapTexture_.GetHeight() - viewH)));

    SDL2pp::Rect src(static_cast<int>(srcX), static_cast<int>(srcY), static_cast<int>(viewW),
                     static_cast<int>(viewH));
    SDL2pp::Rect dst(MINIMAP_MARGIN_X, MINIMAP_MARGIN_Y, miniWidth, miniHeight);

    // Background
    renderer_.Copy(const_cast<SDL2pp::Texture&>(mapTexture_), src, dst);

    // Local scales within the visible area
    float scaleX = static_cast<float>(miniWidth) / viewW;
    float scaleY = static_cast<float>(miniHeight) / viewH;

    // Players
    for (const auto& [id, player]: world.GetPlayers()) {
        float relX = (player.GetX() - srcX) * scaleX;
        float relY = (player.GetY() - srcY) * scaleY;
        int dotX = MINIMAP_MARGIN_X + static_cast<int>(relX);
        int dotY = MINIMAP_MARGIN_Y + static_cast<int>(relY);

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
            int dotX = MINIMAP_MARGIN_X + static_cast<int>(relX);
            int dotY = MINIMAP_MARGIN_Y + static_cast<int>(relY);

            SDL_Color color;

            if (cp.id == activeCp.id) {
                color = {255, 255, 255, 255};  // activo → blanco brillante
            } else if (passed.count(cp.id)) {
                color = {0, 255, 0, 180};  // pasado → verde
            } else {
                color = {180, 180, 180, 150};  // pendiente → gris
            }

            renderer_.SetDrawColor(color.r, color.g, color.b, color.a);
            renderer_.FillRect(SDL2pp::Rect(dotX - 2, dotY - 2, 4, 4));
        }
    }


    // Camera rectangle
    renderer_.SetDrawColor(CAMERA_RECT_COLOR.r, CAMERA_RECT_COLOR.g, CAMERA_RECT_COLOR.b,
                           CAMERA_RECT_COLOR.a);
    SDL2pp::Rect camRect(MINIMAP_MARGIN_X + static_cast<int>((camera.getX() - srcX) * scaleX),
                         MINIMAP_MARGIN_Y + static_cast<int>((camera.getY() - srcY) * scaleY),
                         static_cast<int>(camera.GetViewportWidth() * scaleX),
                         static_cast<int>(camera.GetViewportHeight() * scaleY));
    renderer_.DrawRect(camRect);
}
