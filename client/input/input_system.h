#pragma once
#include <SDL2/SDL.h>

#include "../../common/Dto/dto.h"


class InputSystem {
public:
    void PollEvents(bool& running);
    Dto GetInputByte(int playerId) const;
};
