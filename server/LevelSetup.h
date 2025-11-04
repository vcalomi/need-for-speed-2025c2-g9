#pragma once

#include <box2d/box2d.h>
#include "./physics/LevelCreator.h"
#include "constants.h"   
#include "YamlParser.h"
#include "../common/car_config.h"
#include "./physics/vehicle.h"
#include <algorithm>
#include <random>
#include <map>

class LevelSetup {

    public:
        explicit LevelSetup(const std::string& level_json_path, const std::string& vehicle_specs_path, std::map<int, CarConfig>& chosenCars);
        void buildVehicles(const std::map<int, CarConfig>& chosenCars);
        void step(float fixed_dt = 1.0f/60.0f, int subSteps = 4);
        const std::unordered_map<int, std::unique_ptr<Vehicle>>& getVehicleMap() const { return player_vehicle_map_; }

    private:
        void create_vehicles();

        std::unordered_map<std::string, VehicleSpec> config_map_;
        b2WorldId world_{ b2_nullWorldId };
        LevelCreator  levelCreator_;
        std::vector<Spawn> spawns_;
        std::unordered_map<int, std::unique_ptr<Vehicle>> player_vehicle_map_;
        std::mt19937 rng_;
};

