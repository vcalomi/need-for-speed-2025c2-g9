#ifndef VEHICLE_UPGRADE_DTO_H
#define VEHICLE_UPGRADE_DTO_H
#include <string>
#include <vector>

#include "dto.h"

class VehicleUpgradeDto: public Dto {
public:
    std::string username;
    bool healthUpgrade;
    bool speedUpgrade;

    VehicleUpgradeDto(const std::string& username, bool healthUpgrade, bool speedUpgrade);
};

#endif
