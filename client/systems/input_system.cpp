#include "./input_system.h"

#include "../../common/Dto/player_move.h"
#include "../../common/common_codes.h"

void InputSystem::PollEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
}

PlayerMoveDto InputSystem::GetInputByte(std::string username) const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W])
        return PlayerMoveDto(username, (static_cast<uint8_t>(ActionCode::ACCELERATE)));
    if (keys[SDL_SCANCODE_S])
        return PlayerMoveDto(username, (static_cast<uint8_t>(ActionCode::BRAKE)));
    if (keys[SDL_SCANCODE_A])
        return PlayerMoveDto(username, (static_cast<uint8_t>(ActionCode::TURN_RIGHT)));
    if (keys[SDL_SCANCODE_D])
        return PlayerMoveDto(username, (static_cast<uint8_t>(ActionCode::TURN_LEFT)));

    // Ninguna tecla presionada: no se envía nada
    return PlayerMoveDto(username, (static_cast<uint8_t>(ActionCode::IDLE)));
}
