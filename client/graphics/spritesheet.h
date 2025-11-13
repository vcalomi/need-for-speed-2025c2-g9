#pragma once
#include <map>
#include <string>

#include <SDL2pp/SDL2pp.hh>

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::Texture;


struct Sprite {
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
    SpriteSheet(Renderer& renderer, const std::string& path);

    Texture& GetTexture();

    void AddSprite(const std::string& name, const Rect& area);
    const Sprite& GetSprite(const std::string& name) const;
    bool HasSprite(const std::string& name) const;

private:
    Texture texture_;
    std::map<std::string, Sprite> sprites_;
};
