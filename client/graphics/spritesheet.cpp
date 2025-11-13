#include "./spritesheet.h"

#include <stdexcept>

SpriteSheet::SpriteSheet(Renderer& renderer, const std::string& path): texture_(renderer, path) {}

Texture& SpriteSheet::GetTexture() { return texture_; }

void SpriteSheet::AddSprite(const std::string& name, const Rect& area) {
    Sprite s;
    s.area = area;
    s.width = area.w;
    s.height = area.h;
    s.halfWidth = s.width / 2.0f;
    s.halfHeight = s.height / 2.0f;

    s.frontOffset = s.halfHeight;
    s.backOffset = -s.halfHeight;

    sprites_[name] = s;
}

const Sprite& SpriteSheet::GetSprite(const std::string& name) const {
    auto it = sprites_.find(name);
    if (it == sprites_.end())
        throw std::runtime_error("Sprite not found: " + name);
    return it->second;
}

bool SpriteSheet::HasSprite(const std::string& name) const {
    return sprites_.find(name) != sprites_.end();
}
