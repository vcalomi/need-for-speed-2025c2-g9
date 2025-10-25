#include "./input_system.h"

void InputSystem::PollEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
}

void InputSystem::HandlePlayerInput(Player& player, float delta) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    float accel = 0.0f;
    float turn = 0.0f;

    if (keys[SDL_SCANCODE_W])
        accel = 1.0f;
    if (keys[SDL_SCANCODE_S])
        accel = -1.0f;
    if (keys[SDL_SCANCODE_A])
        turn = -1.0f;
    if (keys[SDL_SCANCODE_D])
        turn = 1.0f;

    player.ApplyInput(accel, turn, delta);
}
