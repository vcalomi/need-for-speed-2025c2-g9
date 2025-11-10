#include "./engine.h"

#include <iostream>

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
        std::cerr << "[Engine] Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        throw std::runtime_error("Failed to initialize SDL_ttf");
    }

    std::cout << "[Engine] SDL_ttf initialized successfully.\n";
}

Engine::~Engine() {
    TTF_Quit();
    std::cout << "[Engine] SDL_ttf cleaned up.\n";
}

Renderer& Engine::GetRenderer() { return renderer_; }

int Engine::getWindowWidth() { return WINDOW_WIDTH; }

int Engine::getWindowHeight() { return WINDOW_HEIGHT; }
