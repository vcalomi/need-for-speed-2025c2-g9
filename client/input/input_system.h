#pragma once
#include <SDL2/SDL.h>


class InputSystem {
public:
    void PollEvents(bool& running);
    uint8_t GetInputByte() const;
};
