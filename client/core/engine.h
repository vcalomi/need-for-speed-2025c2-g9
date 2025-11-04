#pragma once
#include <SDL2pp/SDL2pp.hh>

class Engine {
public:
    Engine();
    SDL2pp::Renderer& GetRenderer();

private:
    SDL2pp::SDL sdl_;
    SDL2pp::Window window_;
    SDL2pp::Renderer renderer_;
};
