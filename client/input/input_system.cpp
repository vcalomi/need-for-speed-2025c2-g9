#include "./input_system.h"

#define INPUT_FORWARD 0x01
#define INPUT_BACKWARD 0x02
#define INPUT_LEFT 0x04
#define INPUT_RIGHT 0x08

void InputSystem::PollEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            running = false;
    }
}

uint8_t InputSystem::GetInputByte() const {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    uint8_t inputByte = 0;

    if (keys[SDL_SCANCODE_W])
        inputByte |= INPUT_FORWARD;
    if (keys[SDL_SCANCODE_S])
        inputByte |= INPUT_BACKWARD;
    if (keys[SDL_SCANCODE_A])
        inputByte |= INPUT_LEFT;
    if (keys[SDL_SCANCODE_D])
        inputByte |= INPUT_RIGHT;

    return inputByte;
}
