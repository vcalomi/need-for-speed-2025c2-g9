#include "vehicle_checkpoint.h"

#include "../common_codes.h"

VehicleCheckpointDto::VehicleCheckpointDto(int vehicleId, int checkpointIndex):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_CHECKPOINT)), vehicleId(vehicleId), checkpointIndex(checkpointIndex) {}
