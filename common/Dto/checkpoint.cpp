#include "checkpoint.h"

#include "../common_codes.h"

CheckpointDto::CheckpointDto(const std::vector<Checkpoint>& checkpoints):
        Dto(uint8_t(ActionCode::SEND_CHECKPOINTS)), checkpoints(checkpoints) {}
