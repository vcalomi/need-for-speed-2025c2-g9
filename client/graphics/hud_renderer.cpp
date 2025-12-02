#include "hud_renderer.h"

#include <algorithm>
#include <string>

HUDRenderer::HUDRenderer(SDL2pp::Renderer& renderer, TextRenderer& text):
        renderer_(renderer), text_(text) {}

void HUDRenderer::Render(const World& world, ProgressManager& progress) {
    RenderLapCounter(world, progress);
    RenderHealthBar(world);
}

void HUDRenderer::RenderLapCounter(const World& world, ProgressManager& progress) {
    const auto& local = world.GetLocalPlayer();

    int laps = progress.GetLapCount();
    int totalLaps = 3;

    int cp = progress.GetActiveCheckpoint().id;
    int totalCp = world.GetCheckpoints().size();

    std::string lapText = "Lap " + std::to_string(laps + 1) + "/" + std::to_string(totalLaps);
    std::string cpText = "CP " + std::to_string(cp) + "/" + std::to_string(totalCp);

    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    text_.Draw(lapText, 20, h - 80);
    text_.Draw(cpText, 20, h - 50);
}

void HUDRenderer::RenderHealthBar(const World& world) {
    const auto& local = world.GetLocalPlayer();

    float health = local.GetHealth();
    float max = local.GetMaxHealth();

    float ratio = std::clamp(max > 0 ? health / max : 0.0f, 0.0f, 1.0f);

    int screenW, screenH;
    SDL_GetRendererOutputSize(renderer_.Get(), &screenW, &screenH);

    int barX = 20;
    int barY = screenH - 130;
    int barW = 200;
    int barH = 18;

    SDL_Rect bg{barX, barY, barW, barH};
    SDL_SetRenderDrawColor(renderer_.Get(), 40, 40, 40, 200);
    SDL_RenderFillRect(renderer_.Get(), &bg);

    SDL_Rect hp{barX, barY, int(barW * ratio), barH};

    if (local.hasInfiniteHealth()) {
        SDL_SetRenderDrawColor(renderer_.Get(), 0, 235, 255, 255);
    } else {
        SDL_SetRenderDrawColor(renderer_.Get(), ratio < 0.3 ? 255 : (ratio < 0.6 ? 255 : 0),
                               ratio < 0.3 ? 40 : (ratio < 0.6 ? 165 : 255), ratio < 0.3 ? 40 : 0, 255);
    }

    SDL_RenderFillRect(renderer_.Get(), &hp);

    std::string hpText = std::to_string((int)health) + "/" + std::to_string((int)max);
    text_.Draw(hpText, barX + 70, barY + 2);
}
