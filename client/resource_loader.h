#pragma once
#include "./spritesheet.h"

class ResourceLoader {
public:
    explicit ResourceLoader(SDL2pp::Renderer& renderer);
    SpriteSheet& GetCarSprites();

private:
    SpriteSheet carSprites_;
    void LoadCarSprites();
};
