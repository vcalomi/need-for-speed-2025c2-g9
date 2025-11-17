#pragma once
#include <algorithm>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../events/event.h"

#include "camera.h"

class Map {
public:
    Map(SDL2pp::Renderer& renderer, EventBus& eventBus);

    void RenderBackground(const Camera& camera);

    void RenderForeground(const Camera& camera);

    int GetWidth() const;
    int GetHeight() const;
    const SDL2pp::Texture& GetTexture() const;

    bool IsLoaded() const;

private:
    SDL2pp::Renderer& renderer_;
    EventBus& eventBus_;
    SDL2pp::Texture backgroundTexture_;
    SDL2pp::Texture foregroundTexture_;
    int width_;
    int height_;
};
