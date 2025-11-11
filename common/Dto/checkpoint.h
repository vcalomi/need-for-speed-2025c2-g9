#ifndef CHECKPOINT_DTO_H
#define CHECKPOINT_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class CheckpointDto: public Dto {
public:
    int id;
    float x;
    float y;

    CheckpointDto(int id, float x, float y);
};

#endif
