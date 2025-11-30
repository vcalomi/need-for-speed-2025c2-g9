#include "vehicle_upgrade.h"

#include "../common_codes.h"

VehicleUpgradeDto:: VehicleUpgradeDto(bool healthUpgrade, bool speedUpgrade):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_UPGRADE)),
        healthUpgrade(healthUpgrade),
        speedUpgrade(speedUpgrade) {}
