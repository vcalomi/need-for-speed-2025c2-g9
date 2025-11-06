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

PlayerMoveDto InputSystem::GetInputByte(int playerId) const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W])
        return PlayerMoveDto(static_cast<uint8_t>(playerId),
                             (static_cast<uint8_t>(ActionCode::ACCELERATE)));
    if (keys[SDL_SCANCODE_S])
        return PlayerMoveDto(static_cast<uint8_t>(playerId),
                             (static_cast<uint8_t>(ActionCode::BRAKE)));
    if (keys[SDL_SCANCODE_A])
        return PlayerMoveDto(static_cast<uint8_t>(playerId),
                             (static_cast<uint8_t>(ActionCode::TURN_LEFT)));
    if (keys[SDL_SCANCODE_D])
        return PlayerMoveDto(static_cast<uint8_t>(playerId),
                             (static_cast<uint8_t>(ActionCode::TURN_RIGHT)));

    // Ninguna tecla presionada: no se envía nada
    return PlayerMoveDto(static_cast<uint8_t>(playerId), (static_cast<uint8_t>(ActionCode::IDLE)));
}
