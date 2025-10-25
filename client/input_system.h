#pragma once
#include <SDL2/SDL.h>

#include "./player.h"

class InputSystem {
public:
    void PollEvents(bool& running);
    void HandlePlayerInput(Player& player, float delta);
};
