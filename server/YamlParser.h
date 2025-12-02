#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "network/race_info.h"
#include "physics/CheckpointInfo.h"
#include "physics/spawn.h"

#include "VehicleSpec.h"

class YamlParser {
public:
    YamlParser() = default;
    ~YamlParser() = default;

    std::unordered_map<std::string, VehicleSpec> parse(const std::string& filename);
    RaceInfo parseRaceInfo(const std::string& filename);
};
