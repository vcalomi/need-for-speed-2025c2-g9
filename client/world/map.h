#pragma once
#include <algorithm>
#include <optional>
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

    bool IsLoaded() const;

    SDL2pp::Texture* GetBackgroundTexture() {
        if (backgroundTexture_)
            return &(*backgroundTexture_);
        return nullptr;
    }

    const SDL2pp::Texture* GetBackgroundTexture() const {
        if (backgroundTexture_)
            return &(*backgroundTexture_);
        return nullptr;
    }


private:
    SDL2pp::Renderer& renderer_;
    EventBus& eventBus_;

    std::optional<SDL2pp::Texture> backgroundTexture_;
    std::optional<SDL2pp::Texture> foregroundTexture_;

    int width_;
    int height_;
};
