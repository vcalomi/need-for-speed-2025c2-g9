#include "./lobbyScreen.h"

#include <iostream>

#include <SDL2/SDL_ttf.h>

using namespace SDL2pp;

LobbyScreen::LobbyScreen(Renderer& renderer, SpriteSheet& cars):
        renderer_(renderer),
        cars_(cars),
        background_(renderer, "../editor/loby/assets/loby_image.jpg"),
        font_("../editor/loby/assets/Tektur_Condensed-Bold.ttf", 36) {
    selectedCar_ = "";
}

void LobbyScreen::AddPlayer(const std::string& name) {
    players_.push_back(name);
    selectedCars_[name] = "";
}

void LobbyScreen::HandleInput(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x = e.button.x;
        int y = e.button.y;

        for (const auto& [name, rect]: carPositions_) {
            if (x >= rect.GetX() && x <= rect.GetX() + rect.GetW() && y >= rect.GetY() &&
                y <= rect.GetY() + rect.GetH()) {
                selectedCar_ = name;
                std::cout << "Car selected: " << name << std::endl;
            }
        }
    }
}

void LobbyScreen::Render() {
    renderer_.Clear();

    int w, h;
    SDL_GetRendererOutputSize(renderer_.Get(), &w, &h);
    Rect dst(0, 0, w, h);
    renderer_.Copy(background_, NullOpt, dst);

    // Mostrar título
    Texture titleTexture(renderer_, font_.RenderText_Blended("Selecciona tu auto",
                                                             SDL_Color{255, 255, 255, 255}));
    renderer_.Copy(titleTexture, NullOpt, Rect(w / 2 - 200, 40, 400, 50));

    // Mostrar autos disponibles
    int spacing = 50;
    int carWidth = 128;
    int carHeight = 64;
    int totalWidth = (carWidth + spacing) * carNames_.size() - spacing;
    int startX = (w - totalWidth) / 2;
    int y = 200;

    carPositions_.clear();
    for (size_t i = 0; i < carNames_.size(); ++i) {
        std::string name = carNames_[i];
        Rect dest(startX + i * (carWidth + spacing), y, carWidth, carHeight);
        carPositions_[name] = dest;

        renderer_.Copy(cars_.GetTexture(), cars_.GetSprite(name), dest);

        // Si está seleccionado, dibujar marco
        if (selectedCar_ == name) {
            SDL_SetRenderDrawColor(renderer_.Get(), 255, 255, 0, 255);
            SDL_Rect outline = {dest.GetX() - 4, dest.GetY() - 4, dest.GetW() + 8, dest.GetH() + 8};
            SDL_RenderDrawRect(renderer_.Get(), &outline);
        }
    }

    renderer_.Present();
}
