#pragma once

#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>

#include "../world/camera.h"
#include "../world/world.h"

#include "text_renderer.h"

enum class UpgradeChoice { NONE, HEALTH, SPEED, CONFIRM };

struct ExplosionAnim {
    float worldX, worldY;
    float size;

    float timePerFrame = 0.03f;
    float elapsed = 0.0f;
    int frame = 0;

    bool finished = false;
};

class ScreenRenderer {
public:
    ScreenRenderer(SDL2pp::Renderer& renderer, TextRenderer& text);

    void RenderExplosion(const Camera& camera, float dt);
    void TriggerExplosion(float worldX, float worldY, float size);

    void RenderPlayerLost();
    void RenderPlayerFinished(int position, float timeSeconds);

    void RenderRaceFinished(const World& world);
    void RenderUpgradeOptions();

    UpgradeChoice HandleUpgradeInput(int mouseX, int mouseY, bool mousePressed);

    bool IsHealthSelected() const { return selectedHealth_; }
    bool IsSpeedSelected() const { return selectedSpeed_; }

    void ResetSelections() {
        selectedHealth_ = false;
        selectedSpeed_ = false;
        upgradesLocked_ = false;
    }

    void LockUpgrades() { upgradesLocked_ = true; }
    bool UpgradesLocked() const { return upgradesLocked_; }


private:
    SDL2pp::Renderer& renderer_;
    TextRenderer& text_;

    std::vector<ExplosionAnim> explosions_;
    std::unique_ptr<SDL2pp::Texture> explosionTexture_;

    SDL_Rect healthButton_;
    SDL_Rect speedButton_;
    SDL_Rect confirmButton_;

    bool selectedHealth_ = false;
    bool selectedSpeed_ = false;
    bool upgradesLocked_ = false;
    bool mouseWasPressed_ = false;
    void DrawCentered(const std::string& msg, int y);
};
