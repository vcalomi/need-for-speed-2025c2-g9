#include "./engine.h"

using SDL2pp::Renderer;

Engine::Engine():
        sdl_(SDL_INIT_VIDEO),
        window_("Need for Speed - Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
                600, SDL_WINDOW_SHOWN),
        renderer_(window_, -1, SDL_RENDERER_ACCELERATED) {}

Renderer& Engine::GetRenderer() { return renderer_; }
