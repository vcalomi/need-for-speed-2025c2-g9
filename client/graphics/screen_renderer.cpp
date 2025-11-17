#include "screen_renderer.h"

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text):
        renderer_(renderer), text_(text) {}


void ScreenRenderer::RenderExplosion(const World& world, float size) {
    const auto& local = world.GetLocalPlayer();
    SDL_FRect boom{local.GetX() - size / 2, local.GetY() - size / 2, size, size};

    SDL_SetRenderDrawColor(renderer_.Get(), 255, 180, 40, 200);
    SDL_RenderFillRectF(renderer_.Get(), &boom);
}

void ScreenRenderer::RenderPlayerLost() {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 180);
    SDL_Rect bg{0, 0, w, h};
    SDL_RenderFillRect(renderer_.Get(), &bg);

    text_.Draw("YOU EXPLODED!", w / 2 - 100, 150);
    text_.Draw("You will appear in the next race.", w / 2 - 150, 220);
    text_.Draw("Waiting for next race...", w / 2 - 120, h - 100);
}

void ScreenRenderer::RenderRaceFinished(const World& world) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 200);
    SDL_Rect bg{0, 0, w, h};
    SDL_RenderFillRect(renderer_.Get(), &bg);

    text_.Draw("RACE FINISHED", w / 2 - 100, 80);
    text_.Draw("Waiting for next race...", w / 2 - 120, h - 100);
}
