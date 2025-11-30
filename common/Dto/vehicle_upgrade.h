#ifndef VEHICLE_UPGRADE_DTO_H
#define VEHICLE_UPGRADE_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleUpgradeDto: public Dto {
public:
    bool healthUpgrade;
    bool speedUpgrade;

    VehicleUpgradeDto(bool healthUpgrade, bool speedUpgrade);
};

#endif
