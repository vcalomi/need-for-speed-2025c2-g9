#include "speedometer.h"

#include <cmath>

Speedometer::Speedometer(SDL2pp::Renderer& renderer, const std::string& dialPath,
                         const std::string& needlePath):
        renderer_(renderer), dial_(renderer, dialPath), needle_(renderer, needlePath) {}

void Speedometer::Render(float speed, float maxSpeed) {
    int sw, sh;
    SDL_GetRendererOutputSize(renderer_.Get(), &sw, &sh);

    int dialSize = 220;
    SDL_Rect dialDest{sw - dialSize - 20, sh - dialSize - 20, dialSize, dialSize};
    renderer_.Copy(dial_, SDL2pp::NullOpt, dialDest);

    float t = std::clamp(speed / maxSpeed, 0.0f, 1.0f);
    float angle = angleMin_ + t * (angleMax_ - angleMin_);

    angle -= 48.85f;

    int centerX = dialDest.x + dialSize / 2;
    int centerY = dialDest.y + dialSize / 2;

    int needleW = dialSize * 0.07f;
    int needleH = dialSize * 0.55f;

    SDL_Rect needleDest{centerX - needleW / 2, centerY - needleH + needleW, needleW, needleH};

    SDL_Point pivot{needleW / 2, needleH - needleW};

    SDL_RenderCopyEx(renderer_.Get(), needle_.Get(), nullptr, &needleDest, angle, &pivot,
                     SDL_FLIP_NONE);
}
