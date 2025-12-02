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
    SpriteSheet& GetNpcSprites();

private:
    void LoadCarSprites();
    void LoadNpcSprites();

    SDL2pp::Renderer& renderer_;
    SpriteSheet carSprites_;
    SpriteSheet npcSprites_;
};
