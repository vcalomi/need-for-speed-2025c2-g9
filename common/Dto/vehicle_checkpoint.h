#ifndef VEHICLE_CHECKPOINT_DTO_H
#define VEHICLE_CHECKPOINT_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleCheckpointDto: public Dto {
public:
    int vehicleId; 
    int checkpointIndex;

    VehicleCheckpointDto(int vehicleId, int checkpointIndex);
};

#endif
