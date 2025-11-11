#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <box2d/box2d.h>

#include "../common/car_config.h"
#include "./physics/LevelCreator.h"
#include "./physics/vehicle.h"

#include "YamlParser.h"
#include "constants.h"

class LevelSetup {

public:
    explicit LevelSetup(const std::string& level_json_path, const std::string& vehicle_specs_path,
                        const std::map<int, CarConfig>& chosenCars);

    void buildVehicles();
    void step(float fixed_dt = 1.0f / 60.0f, int subSteps = 4);
    
    const std::unordered_map<int, std::unique_ptr<Vehicle>>& getVehicleMap() const {
        return player_vehicle_map_;
    }

    const std::vector<CheckpointInfo>& getCheckpoints() const  { return checkpoints_;}


private:
    void create_vehicles();
    const std::map<int, CarConfig>& chosenCarsRef_; 
    std::unordered_map<std::string, VehicleSpec> config_map_;
    b2WorldId world_{b2_nullWorldId};
    LevelCreator levelCreator_;
    std::vector<Spawn> spawns_;
    std::vector<CheckpointInfo> checkpoints_;
    std::unordered_map<int, std::unique_ptr<Vehicle>> player_vehicle_map_;
    std::mt19937 rng_;
    std::vector<FixtureTag> vehicle_tags_;
};
