#include "./spritesheet.h"

#include <stdexcept>

SpriteSheet::SpriteSheet(Renderer& renderer, const std::string& path): texture_(renderer, path) {}

Texture& SpriteSheet::GetTexture() { return texture_; }

void SpriteSheet::AddSprite(const std::string& name, const Rect& area) {
    sprites_.emplace(name, area);
}

const Rect& SpriteSheet::GetSprite(const std::string& name) const {
    auto it = sprites_.find(name);
    if (it == sprites_.end())
        throw std::runtime_error("Sprite not found: " + name);
    return it->second;
}

bool SpriteSheet::HasSprite(const std::string& name) const {
    return sprites_.find(name) != sprites_.end();
}
