#include "vehicle_checkpoint.h"

#include "../common_codes.h"

VehicleCheckpointDto::VehicleCheckpointDto(const std::string& username, int checkpointIndex):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_CHECKPOINT)), username(username), checkpointIndex(checkpointIndex) {}
