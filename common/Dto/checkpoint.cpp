#include "checkpoint.h"

#include "../common_codes.h"

CheckpointDto::CheckpointDto(int id, float x, float y):
        Dto(uint8_t(ActionCode::SEND_CHECKPOINTS)), id(id), x(x), y(y) {}
