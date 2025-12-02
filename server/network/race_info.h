#pragma once

#include <string>
#include <vector>

#include "../physics/CheckpointInfo.h"
#include "../physics/spawn.h"

struct RaceInfo {
    std::string mapName;  // "liberty_city", "san_andreas", "vice_city"
    std::vector<CheckpointInfo> checkpoints;
    std::vector<Spawn> spawns;
};
