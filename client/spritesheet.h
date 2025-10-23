#include <map>
#include <string>

#include <SDL2pp/SDL2pp.hh>

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::Texture;

class SpriteSheet {
public:
    SpriteSheet(Renderer& renderer, const std::string& path);

    Texture& GetTexture();

    void AddSprite(const std::string& name, const Rect& area);
    const Rect& GetSprite(const std::string& name) const;

private:
    Texture texture_;
    std::map<std::string, SDL2pp::Rect> sprites_;
};
