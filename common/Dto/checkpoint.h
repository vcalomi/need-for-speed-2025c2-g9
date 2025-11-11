#ifndef CHECKPOINT_DTO_H
#define CHECKPOINT_DTO_H
#include <string>
#include <vector>

#include "dto.h"

struct Checkpoint {
    float x;
    float y;

    Checkpoint(float x, float y): x(x), y(y) {}
};

class CheckpointDto: public Dto {
public:
    std::vector<Checkpoint> checkpoints;

    CheckpointDto(const std::vector<Checkpoint>& checkpoints);
};

#endif
