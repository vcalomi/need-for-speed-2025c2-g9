#include "screen_renderer.h"

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text):
        renderer_(renderer),
        text_(text),
        explosionTexture_(std::make_unique<SDL2pp::Texture>(
                renderer, "../client/assets/need-for-speed/cars/explosion.png")) {}


void ScreenRenderer::RenderExplosion(const Camera& camera, float dt) {
    if (!explosionTexture_)
        return;

    const int rows = 6;
    const int cols = 8;
    const int totalFrames = rows * cols;

    int texW, texH;
    SDL_QueryTexture(explosionTexture_->Get(), nullptr, nullptr, &texW, &texH);

    const int frameW = texW / cols;
    const int frameH = texH / rows;

    for (auto& e: explosions_) {
        if (e.finished)
            continue;

        e.elapsed += dt;
        if (e.elapsed >= e.timePerFrame) {
            e.elapsed = 0;
            e.frame++;

            if (e.frame >= totalFrames) {
                e.finished = true;
                continue;
            }
        }

        int row = e.frame / cols;
        int col = e.frame % cols;

        SDL_Rect src{col * frameW, row * frameH, frameW, frameH};

        SDL2pp::Rect dst(static_cast<int>(e.worldX - camera.getX() - e.size / 2),
                         static_cast<int>(e.worldY - camera.getY() - e.size / 2),
                         static_cast<int>(e.size), static_cast<int>(e.size));

        renderer_.Copy(*explosionTexture_, SDL2pp::Optional<SDL2pp::Rect>(src),
                       SDL2pp::Optional<SDL2pp::Rect>(dst));


        renderer_.Copy(*explosionTexture_, src, dst);
    }

    // borrar explosiones finalizadas
    explosions_.erase(std::remove_if(explosions_.begin(), explosions_.end(),
                                     [](const auto& e) { return e.finished; }),
                      explosions_.end());
}


void ScreenRenderer::TriggerExplosion(float worldX, float worldY, float size) {
    explosions_.push_back({worldX, worldY, size});
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
