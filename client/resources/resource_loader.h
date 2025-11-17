#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>
#include <yaml-cpp/yaml.h>

#include "../graphics/spritesheet.h"

class ResourceLoader {
public:
    explicit ResourceLoader(SDL2pp::Renderer& renderer);

    SpriteSheet& GetCarSprites();

private:
    void LoadCarSprites();

    SDL2pp::Renderer& renderer_;
    SpriteSheet carSprites_;
};
