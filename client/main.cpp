#include <exception>
#include <iostream>
#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLImage.hh>
#include <SDL2pp/Texture.hh>

#include "./lobbyScreen.h"
#include "./spritesheet.h"

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::Texture;
using SDL2pp::Window;


int main() try {
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::SDLTTF ttf;
    SDL2pp::Window window("Lobby", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                          SDL_WINDOW_RESIZABLE);
    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SpriteSheet cars(renderer, "../client/assets/need-for-speed/cars/cars.png");
    cars.AddSprite("red", {0, 0, 128, 64});
    cars.AddSprite("blue", {128, 0, 128, 64});
    cars.AddSprite("green", {256, 0, 128, 64});

    LobbyScreen lobby(renderer, cars);
    lobby.AddPlayer("Player1");
    lobby.AddPlayer("Player2");

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
            lobby.HandleInput(e);
        }

        lobby.Render();
        SDL_Delay(16);  // ~60 FPS
    }

    return 0;
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}