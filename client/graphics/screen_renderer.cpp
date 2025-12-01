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
    int sw, sh;
    SDL_GetRendererOutputSize(renderer_.Get(), &sw, &sh);

    SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 160);
    SDL_Rect bg{0, 0, sw, sh};
    SDL_RenderFillRect(renderer_.Get(), &bg);
    DrawCentered("RACE FINISHED!", sh / 4);

    int y = sh / 4 + 80;
    // int pos = 1;
    // for (const auto& [id, player]: world.GetPlayers()) {
    //     std::string line = std::to_string(pos) + ". " + id + "  -  " +
    //                        std::to_string(player.GetFinishTime()) + "s";
    //     DrawCentered(line, y);
    //     y += 30;
    //     pos++;
    // }

    RenderUpgradeOptions();
}

void ScreenRenderer::RenderCountdown(float countdownTimer, int countdownNumber) {
    std::string text;

    if (countdownNumber > 0) {
        text = std::to_string(countdownNumber);
    } else {
        text = "GO!";
    }

    int fontSize = 180;
    SDL_Color color = {255, 255, 255, 255};


    if (countdownNumber == 0) {
        float t = std::clamp(countdownTimer * -1.0f, 0.0f, 1.0f);
        color.a = (Uint8)((1.0f - t) * 255);
    }

    RenderCenteredText(text, fontSize, color);
}

void ScreenRenderer::RenderCenteredText(const std::string& text, int size, SDL_Color color) {
    SDL2pp::Font font("../client/lobby/assets/Tektur-SemiBold.ttf", size);
    SDL2pp::Surface surface(font.RenderText_Blended(text, color));
    SDL2pp::Texture texture(renderer_, surface);

    int w, h;
    SDL_QueryTexture(texture.Get(), nullptr, nullptr, &w, &h);

    int sw, sh;
    SDL_GetRendererOutputSize(renderer_.Get(), &sw, &sh);

    SDL_Rect dst{(sw - w) / 2, (sh - h) / 2, w, h};

    renderer_.Copy(texture, SDL2pp::NullOpt, dst);
}


void ScreenRenderer::RenderUpgradeOptions() {


    int sw, sh;
    SDL_GetRendererOutputSize(renderer_.Get(), &sw, &sh);

    int btnWidth = 300;
    int btnHeight = 60;
    int x = (sw - btnWidth) / 2;

    int yHealth = sh - 260;
    int ySpeed = sh - 180;
    int yConfirm = sh - 80;

    if (upgradesLocked_) {
        DrawCentered("Upgrades confirmed!", sh - 150);
        return;
    }

    healthButton_ = {x, yHealth, btnWidth, btnHeight};
    speedButton_ = {x, ySpeed, btnWidth, btnHeight};
    confirmButton_ = {x, yConfirm, btnWidth, btnHeight};

    auto drawButton = [&](SDL_Rect r, bool selected, const std::string& txt) {
        if (selected)
            renderer_.SetDrawColor(60, 180, 60, 200);  // verde si seleccionado
        else
            renderer_.SetDrawColor(80, 80, 80, 180);

        renderer_.FillRect(r);
        text_.Draw(txt, r.x + 15, r.y + 20);
    };

    drawButton(healthButton_, selectedHealth_, "Improve Health (+3s)");
    drawButton(speedButton_, selectedSpeed_, "Improve Speed (+3s)");
    drawButton(confirmButton_, false, "Confirm Upgrades");
}


UpgradeChoice ScreenRenderer::HandleUpgradeInput(int mx, int my, bool mousePressed) {
    if (upgradesLocked_) {
        mouseWasPressed_ = mousePressed;
        return UpgradeChoice::NONE;
    }

    bool justClicked = (mousePressed && !mouseWasPressed_);
    mouseWasPressed_ = mousePressed;

    if (!justClicked)
        return UpgradeChoice::NONE;

    SDL_Point p{mx, my};

    if (SDL_PointInRect(&p, &healthButton_)) {
        selectedHealth_ = !selectedHealth_;
        return UpgradeChoice::HEALTH;
    }

    if (SDL_PointInRect(&p, &speedButton_)) {
        selectedSpeed_ = !selectedSpeed_;
        return UpgradeChoice::SPEED;
    }

    if (SDL_PointInRect(&p, &confirmButton_)) {
        return UpgradeChoice::CONFIRM;
    }

    return UpgradeChoice::NONE;
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
