#include "./spritesheet.h"

#include <stdexcept>

void SpriteSheet::AddSprite(const std::string& name, std::shared_ptr<Texture> texture,
                            const Rect& area) {
    Sprite s;
    s.texture = texture.get();
    s.area = area;

    s.width = area.w;
    s.height = area.h;
    s.halfWidth = s.width / 2.0f;
    s.halfHeight = s.height / 2.0f;

    s.frontOffset = s.halfHeight;
    s.backOffset = -s.halfHeight;

    sprites_[name] = s;

    textures_.push_back(texture);
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
