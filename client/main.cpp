#include <exception>
#include <iostream>
#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/SDLImage.hh>
#include <SDL2pp/Texture.hh>

#include "common/foo.h"

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::Texture;
using SDL2pp::Window;


int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);


    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("Need for speed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                  SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Texture texture(renderer, "../client/assets/need-for-speed/cars/cars.png");

    std::map<std::string, Rect> cars = {{"red", Rect(0, 64, 64, 64)},
                                        {"blue", Rect(64, 128, 64, 64)},
                                        {"green", Rect(128, 0, 64, 64)}};

    SDL2pp::Rect srcRect(3 * 64, 2 * 64, 64, 64);
    SDL2pp::Rect destRect(100, 100, 64, 64);

    renderer.Clear();
    renderer.Copy(texture, cars["red"], Rect(100, 100, 64, 64));
    renderer.Copy(texture, cars["blue"], Rect(200, 100, 64, 64));
    renderer.Copy(texture, cars["green"], Rect(300, 100, 64, 64));
    renderer.Present();
    SDL_Delay(5000);


    // Here all resources are automatically released and library deinitialized
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
