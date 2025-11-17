#include "map.h"

#include "../events/race_info_event.h"

Map::Map(SDL2pp::Renderer& renderer, EventBus& eventBus):
        renderer_(renderer),
        eventBus_(eventBus),
        backgroundTexture_(nullptr),
        foregroundTexture_(nullptr),
        width_(0),
        height_(0) {

    eventBus_.Subscribe<RaceInfoEvent>([this](const RaceInfoEvent& e) {
        std::string backgroundpath = e.map;
        std::string foregroundpath = e.map + "_foreground";
        backgroundTexture_ = SDL2pp::Texture(renderer_, backgroundpath);
        foregroundTexture_ = SDL2pp::Texture(renderer_, foregroundpath);

        SDL2pp::Point size = backgroundTexture_.GetSize();
        width_ = size.x;
        height_ = size.y;
    });
}

void Map::RenderBackground(const Camera& camera) {
    SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
    renderer_.Copy(backgroundTexture_, SDL2pp::NullOpt, dst);
}

void Map::RenderForeground(const Camera& camera) {
    SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
    renderer_.Copy(foregroundTexture_, SDL2pp::NullOpt, dst);
}

int Map::GetWidth() const { return width_; }
int Map::GetHeight() const { return height_; }
const SDL2pp::Texture& Map::GetTexture() const { return backgroundTexture_; }

bool Map::IsLoaded() const {
    return backgroundTexture_.Get() != nullptr && foregroundTexture_.Get() != nullptr;
}
