#include "./engine.h"

using SDL2pp::Renderer;

#define WINDOW_TITLE "Need for Speed - Simulation"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Engine::Engine():
        sdl_(SDL_INIT_VIDEO),
        window_(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                WINDOW_HEIGHT, SDL_WINDOW_SHOWN),
        renderer_(window_, -1, SDL_RENDERER_ACCELERATED) {
    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize SDL_ttf");
    }
}

Engine::~Engine() { TTF_Quit(); }

Renderer& Engine::GetRenderer() { return renderer_; }

int Engine::getWindowWidth() { return WINDOW_WIDTH; }

int Engine::getWindowHeight() { return WINDOW_HEIGHT; }
