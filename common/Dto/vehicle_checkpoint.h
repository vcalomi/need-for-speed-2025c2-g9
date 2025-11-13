#ifndef VEHICLE_CHECKPOINT_DTO_H
#define VEHICLE_CHECKPOINT_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleCheckpointDto: public Dto {
public:
    std::string username;  
    int checkpointIndex;

    VehicleCheckpointDto(const std::string& username, int checkpointIndex);
};

#endif
