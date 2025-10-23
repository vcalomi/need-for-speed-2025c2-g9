#pragma once
#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLTTF.hh>

#include "./spritesheet.h"

class LobbyScreen {
public:
    LobbyScreen(SDL2pp::Renderer& renderer, SpriteSheet& cars);

    void AddPlayer(const std::string& name);
    void HandleInput(const SDL_Event& e);
    void Render();

private:
    SDL2pp::Renderer& renderer_;
    SpriteSheet& cars_;
    SDL2pp::Texture background_;
    SDL2pp::Font font_;
    std::map<std::string, SDL2pp::Rect> carPositions_;
    std::map<std::string, std::string> selectedCars_;
    std::vector<std::string> carNames_ = {"red", "blue", "green"};
    std::vector<std::string> players_;
    std::string selectedCar_;
};
