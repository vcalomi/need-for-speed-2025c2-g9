#pragma once
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "../../common/Dto/player_move.h"

class InputSystem {
public:
    void PollEvents(bool& running);
    PlayerMoveDto GetInputByte(std::string username) const;
    bool IsKeyPressed(SDL_Scancode key) const;

    bool WasKeyPressed(SDL_Scancode key);

private:
    const Uint8* keyboardState_ = nullptr;

    std::unordered_map<SDL_Scancode, bool> previousKeyState_;
};
