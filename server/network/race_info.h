#pragma once

struct RaceInfo {
    std::string mapName;                 // "liberty_city", "san_andreas", "vice_city"
    std::vector<CheckpointInfo> checkpoints;
    std::vector<Spawn> spawns;
};
