#include "./game.h"

Game::Game():
        engine_(),
        resources_(engine_.GetRenderer()),
        world_(),
        inputSystem_(),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites()),
        map_(engine_.GetRenderer(), "../client/assets/need-for-speed/cities/san_andreas.png") {

    // Inicialización del estado inicial del mundo
    // En el futuro esto se hará con mensajes del servidor
    world_.AddPlayer("1", "CamionetaRoja", 400.f, 300.f, true);
    world_.AddPlayer("2", "CamionetaRoja", 500.f, 300.f, false);
}

void Game::Run() {
    bool running = true;
    Uint64 prevTicks = SDL_GetTicks64();

    while (running) {
        Uint64 currentTicks = SDL_GetTicks64();
        float delta = (currentTicks - prevTicks) / 1000.f;
        prevTicks = currentTicks;

        inputSystem_.PollEvents(running);
        inputSystem_.HandlePlayerInput(world_.GetLocalPlayer(), delta);

        world_.Update(delta);
        rendererSystem_.Render(world_, map_);

        SDL_Delay(16);
    }
}
