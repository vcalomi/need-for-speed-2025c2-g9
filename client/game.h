#pragma once
#include <SDL2pp/SDL2pp.hh>

#include "./player.h"
#include "./spritesheet.h"

class Game {
public:
    Game();
    void Run();

private:
    SDL2pp::SDL sdl_;
    SDL2pp::Window window_;
    SDL2pp::Renderer renderer_;
    SpriteSheet cars_;
    Player player_;
};
