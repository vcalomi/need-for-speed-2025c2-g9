#include "./game.h"

using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::Texture;
using SDL2pp::Window;

Game::Game():
        sdl_(SDL_INIT_VIDEO),
        window_("Need for Speed - Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
                600, SDL_WINDOW_SHOWN),
        renderer_(window_, -1, SDL_RENDERER_ACCELERATED),
        cars_(renderer_, "../client/assets/need-for-speed/cars/cars.png"),
        localPlayerId_("1") {

    // Cargar sprites
    cars_.AddSprite("CamionetaRojaSur", {167, 304, 26, 39});
    cars_.AddSprite("CamionetaRojaNorte", {167, 345, 26, 39});
    cars_.AddSprite("CamionetaRojaEste", {0, 311, 39, 26});
    cars_.AddSprite("CamionetaRojaOeste", {1, 351, 39, 26});
    cars_.AddSprite("CamionetaRojaSureste20", {40, 307, 40, 36});
    cars_.AddSprite("CamionetaRojaSuroeste20", {280, 307, 40, 36});
    cars_.AddSprite("CamionetaRojaNoroeste20", {40, 345, 40, 36});
    cars_.AddSprite("CamionetaRojaNoreste20", {280, 345, 40, 36});
    cars_.AddSprite("CamionetaRojaSureste45", {80, 304, 40, 40});
    cars_.AddSprite("CamionetaRojaSuroeste45", {240, 304, 40, 40});
    cars_.AddSprite("CamionetaRojaNoroeste45", {80, 344, 40, 40});
    cars_.AddSprite("CamionetaRojaNoreste45", {240, 344, 40, 40});
    cars_.AddSprite("CamionetaRojaSuroeste75", {201, 304, 36, 40});
    cars_.AddSprite("CamionetaRojaSureste75", {123, 304, 36, 40});
    cars_.AddSprite("CamionetaRojaNoreste75", {203, 344, 36, 40});
    cars_.AddSprite("CamionetaRojaNoroeste75", {121, 344, 36, 40});

    players_.emplace("1", Player("1", "CamionetaRoja", 400.0f, 300.0f));
    players_.emplace("2", Player("2", "CamionetaRoja", 500.0f, 300.0f));
}

void Game::Run() {
    bool running = true;
    SDL_Event e;
    Uint64 prevTicks = SDL_GetTicks64();

    while (running) {
        Uint64 currentTicks = SDL_GetTicks64();
        float delta = (currentTicks - prevTicks) / 1000.0f;
        prevTicks = currentTicks;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        // Actualizar solo el jugador local con input
        Player& local = players_.at(localPlayerId_);
        local.HandleInput(keys, delta);
        local.Update(delta);

        // Actualizar los demás (por ahora simulado)
        for (auto& [id, player]: players_) {
            if (id == localPlayerId_)
                continue;
            // En el futuro, acá vas a usar datos recibidos del servidor
            player.Update(delta);
        }

        // Dibujar todos
        renderer_.Clear();
        for (auto& [id, player]: players_) player.Render(renderer_, cars_);
        renderer_.Present();

        SDL_Delay(16);
    }
}
