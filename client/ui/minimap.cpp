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

Minimap::Minimap(SDL2pp::Renderer& renderer, int width, int height):
        renderer_(renderer),
        minimapTexture_(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width,
                        height),
        miniWidth(width),
        miniHeight(height) {}


void Minimap::Render(const World& world, const Camera& camera, const Map& map,
                     ProgressManager& progress) {

    SDL2pp::Texture* mapTexture = const_cast<SDL2pp::Texture*>(map.GetBackgroundTexture());
    if (!mapTexture)
        return;

    const int mapW = mapTexture->GetWidth();
    const int mapH = mapTexture->GetHeight();

    const float viewW = camera.GetViewportWidth() * MINIMAP_ZOOM_FACTOR;
    const float viewH = camera.GetViewportHeight() * MINIMAP_ZOOM_FACTOR;

    float srcX = camera.getX() + camera.GetViewportWidth() / 2 - viewW / 2;
    float srcY = camera.getY() + camera.GetViewportHeight() / 2 - viewH / 2;

    srcX = std::clamp(srcX, 0.0f, (float)(mapW - viewW));
    srcY = std::clamp(srcY, 0.0f, (float)(mapH - viewH));

    SDL2pp::Rect srcRect((int)srcX, (int)srcY, (int)viewW, (int)viewH);

    renderer_.SetTarget(minimapTexture_);
    renderer_.SetDrawColor(0, 0, 0, 255);
    renderer_.Clear();

    renderer_.Copy(*mapTexture, srcRect, SDL2pp::Rect(0, 0, miniWidth, miniHeight));

    float scaleX = miniWidth / viewW;
    float scaleY = miniHeight / viewH;

    for (const auto& [id, player]: world.GetPlayers()) {
        if (player.isExploded())
            continue;

        float relX = (player.GetX() - srcX) * scaleX;
        float relY = (player.GetY() - srcY) * scaleY;

        if (relX < 0 || relY < 0 || relX > miniWidth || relY > miniHeight)
            continue;

        const SDL_Color& color =
                (player.GetUsername() == world.GetLocalPlayer().GetUsername() ? LOCAL_PLAYER_COLOR :
                                                                                OTHER_PLAYER_COLOR);

        renderer_.SetDrawColor(color.r, color.g, color.b, color.a);
        renderer_.FillRect(SDL2pp::Rect((int)relX - PLAYER_DOT_HALF, (int)relY - PLAYER_DOT_HALF,
                                        PLAYER_DOT_SIZE, PLAYER_DOT_SIZE));
    }

    const auto& activeCp = progress.GetActiveCheckpoint();
    const auto passed = progress.GetPassedCheckpoints();

    for (const auto& cp: world.GetCheckpoints()) {
        float relX = (cp.x - srcX) * scaleX;
        float relY = (cp.y - srcY) * scaleY;

        if (relX < 0 || relY < 0 || relX > miniWidth || relY > miniHeight)
            continue;

        SDL_Color color;

        if (cp.id == activeCp.id)
            color = {255, 255, 255, 255};
        else if (passed.count(cp.id))
            color = {0, 255, 0, 180};
        else
            color = {180, 180, 180, 150};

        renderer_.SetDrawColor(color.r, color.g, color.b, color.a);
        renderer_.FillRect(SDL2pp::Rect((int)relX - 2, (int)relY - 2, 4, 4));
    }

    renderer_.SetDrawColor(CAMERA_RECT_COLOR.r, CAMERA_RECT_COLOR.g, CAMERA_RECT_COLOR.b,
                           CAMERA_RECT_COLOR.a);

    SDL2pp::Rect camRect(
            (int)((camera.getX() - srcX) * scaleX), (int)((camera.getY() - srcY) * scaleY),
            (int)(camera.GetViewportWidth() * scaleX), (int)(camera.GetViewportHeight() * scaleY));

    renderer_.DrawRect(camRect);

    renderer_.SetTarget();

    renderer_.Copy(minimapTexture_, SDL2pp::NullOpt,
                   SDL2pp::Rect(MINIMAP_MARGIN_X, MINIMAP_MARGIN_Y, miniWidth, miniHeight));
}
