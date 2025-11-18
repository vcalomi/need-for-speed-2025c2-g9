#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::Texture;

struct Sprite {
    Texture* texture;
    Rect area;

    float width;
    float height;
    float halfWidth;
    float halfHeight;
    float frontOffset;
    float backOffset;
};

class SpriteSheet {
public:
    SpriteSheet() = default;

    void AddSprite(const std::string& name, std::shared_ptr<Texture> texture, const Rect& area);
    const Sprite& GetSprite(const std::string& name) const;
    bool HasSprite(const std::string& name) const;

private:
    std::map<std::string, Sprite> sprites_;
    std::vector<std::shared_ptr<Texture>> textures_;
};
