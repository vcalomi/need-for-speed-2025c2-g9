#include "map.h"

#include <fstream>

#include "../events/race_info_event.h"

static bool FileExists(const std::string& p) { return std::ifstream(p).good(); }

Map::Map(SDL2pp::Renderer& renderer, EventBus& eventBus):
        renderer_(renderer), eventBus_(eventBus), width_(0), height_(0) {
    eventBus_.Subscribe<RaceInfoEvent>([this](const RaceInfoEvent& e) {
        std::string installedAssetsPath = std::string(ASSETS_DIR) + "/cities/";
        std::string sourceAssetsPath = std::string(PROJECT_SOURCE_DIR) + "/client/assets/need-for-speed/cities/";
        std::string backgroundpath = installedAssetsPath + e.map;
        std::string base = e.map;
        if (base.size() > 4 && base.substr(base.size() - 4) == ".png") {
            base = base.substr(0, base.size() - 4);
        }

        std::string foregroundpath = installedAssetsPath + base + "_foreground.png";

        if (!FileExists(backgroundpath)) {
            // fallback a assets del proyecto si no está instalado
            backgroundpath = sourceAssetsPath + e.map;
        }

        if (FileExists(backgroundpath)) {
            backgroundTexture_.emplace(renderer_, backgroundpath);

            SDL2pp::Point size = backgroundTexture_->GetSize();
            width_ = size.x;
            height_ = size.y;
        } else {
            std::cerr << "[Map] ERROR: Background not found: " << backgroundpath << "\n";
            return;
        }

        if (!FileExists(foregroundpath)) {
            foregroundpath = sourceAssetsPath + base + "_foreground.png";
        }

        if (FileExists(foregroundpath)) {
            foregroundTexture_.emplace(renderer_, foregroundpath);
        } else {
            std::cerr << "[Map] No foreground found for: " << e.map << "\n";
            foregroundTexture_.reset();
        }
    });
}

void Map::RenderBackground(const Camera& camera) {

    int rw, rh;
    SDL_GetRendererOutputSize(renderer_.Get(), &rw, &rh);

    if (!backgroundTexture_)
        return;

    SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
    renderer_.Copy(*backgroundTexture_, SDL2pp::NullOpt, dst);
}

void Map::RenderForeground(const Camera& camera) {
    if (!foregroundTexture_)
        return;

    SDL2pp::Rect dst(-camera.getX(), -camera.getY(), width_, height_);
    renderer_.Copy(*foregroundTexture_, SDL2pp::NullOpt, dst);
}

int Map::GetWidth() const { return width_; }
int Map::GetHeight() const { return height_; }

bool Map::IsLoaded() const { return backgroundTexture_.has_value(); }
