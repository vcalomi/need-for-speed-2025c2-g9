#pragma once
#include <string>

#include <SDL2/SDL.h>

#include "../../common/Dto/player_move.h"


class InputSystem {
public:
    void PollEvents(bool& running);
    PlayerMoveDto GetInputByte(std::string username) const;
};
