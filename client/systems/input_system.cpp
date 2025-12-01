#include "./input_system.h"

#include "../../common/Dto/player_move.h"
#include "../../common/common_codes.h"

void InputSystem::PollEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }

    keyboardState_ = SDL_GetKeyboardState(NULL);
}

PlayerMoveDto InputSystem::GetInputByte(std::string username) const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    uint8_t mask = static_cast<uint8_t>(MoveMask::NONE);

    if (keys[SDL_SCANCODE_W])
        mask |= static_cast<uint8_t>(MoveMask::ACCELERATE);

    if (keys[SDL_SCANCODE_S])
        mask |= static_cast<uint8_t>(MoveMask::BRAKE);

    if (keys[SDL_SCANCODE_A])
        mask |= static_cast<uint8_t>(MoveMask::TURN_RIGHT);

    if (keys[SDL_SCANCODE_D])
        mask |= static_cast<uint8_t>(MoveMask::TURN_LEFT);

    return PlayerMoveDto(username, mask);
}

bool InputSystem::IsKeyPressed(SDL_Scancode key) const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    return keys[key] != 0;
}

bool InputSystem::WasKeyPressed(SDL_Scancode key) {
    bool isPressed = IsKeyPressed(key);
    bool wasPressedBefore = previousKeyState_[key];

    bool justPressed = (isPressed && !wasPressedBefore);

    previousKeyState_[key] = isPressed;

    return justPressed;
}