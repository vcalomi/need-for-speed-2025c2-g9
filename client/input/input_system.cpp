#include "./input_system.h"

#include "../../common/common_codes.h"

void InputSystem::PollEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
}

Dto InputSystem::GetInputByte() const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W])
        return Dto(static_cast<uint8_t>(ActionCode::INPUT_NORTH));
    if (keys[SDL_SCANCODE_S])
        return Dto(static_cast<uint8_t>(ActionCode::INPUT_SOUTH));
    if (keys[SDL_SCANCODE_A])
        return Dto(static_cast<uint8_t>(ActionCode::INPUT_WEST));
    if (keys[SDL_SCANCODE_D])
        return Dto(static_cast<uint8_t>(ActionCode::INPUT_EAST));

    // Ninguna tecla presionada: no se envía nada
    return Dto(0);
}
