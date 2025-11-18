#include "screen_renderer.h"

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text):
        renderer_(renderer), text_(text) {}


void ScreenRenderer::RenderExplosion(const World& world, const Camera& camera, float size) {
    const auto& local = world.GetLocalPlayer();

    float screenX = local.GetX() - camera.getX();
    float screenY = local.GetY() - camera.getY();

    SDL_FRect boom{screenX - size / 2, screenY - size / 2, size, size};
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

    DrawCentered("RACE FINISHED", 60);

    // Si el mundo tuviera resultados finales del servidor:
    // auto results = world.GetFinalResults();
    // int y = 120;
    // for (const auto& r : results) {
    //     std::string line = std::to_string(r.position) + ". " + r.username + " - " +
    //     Format(r.time); DrawCentered(line, y); y += 40;
    // }

    DrawCentered("Waiting for next race...", h - 100);
}

void ScreenRenderer::DrawCentered(const std::string& msg, int y) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    int textW = 0, textH = 0;
    text_.Measure(msg, textW, textH);

    text_.Draw(msg, w / 2 - textW / 2, y);
}

void ScreenRenderer::RenderPlayerFinished(int position, float timeSeconds) {
    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 180);
    SDL_Rect bg{0, 0, w, h};
    SDL_RenderFillRect(renderer_.Get(), &bg);

    DrawCentered("YOU FINISHED!", 80);

    std::string pos = "Position: " + std::to_string(position);
    DrawCentered(pos, 150);

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "Time: %.2f s", timeSeconds);
    DrawCentered(buffer, 200);

    DrawCentered("Waiting for all players to finish...", h - 120);
}
