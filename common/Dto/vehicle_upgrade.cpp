#include "vehicle_upgrade.h"

#include "../common_codes.h"

VehicleUpgradeDto:: VehicleUpgradeDto(const std::string& username, bool healthUpgrade, bool speedUpgrade):
        Dto(uint8_t(ActionCode::SEND_VEHICLE_UPGRADE)),
        username(username),
        healthUpgrade(healthUpgrade),
        speedUpgrade(speedUpgrade) {}
