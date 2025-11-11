#include "LevelSetup.h"
#include "./physics/EntityTags.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

LevelSetup::LevelSetup(const std::string& level_json_path, 
                    const std::string& vehicle_specs_path,
                    const std::map<int, CarConfig>& chosenCars):
       chosenCarsRef_(chosenCars), 
       config_map_(YamlParser{}.parse(vehicle_specs_path)) 
{

    // Create empty world
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    world_ = b2CreateWorld(&wdef);

    // Create level hitboxes
    levelCreator_.processDirectoryLevel(level_json_path);
    levelCreator_.createLevelCollision(world_, levelCreator_.levels());

    spawns_ = levelCreator_.getSpawnPoints();
    buildVehicles();
}

void LevelSetup::buildVehicles() {
    std::cout << "[LevelSetup] chosenCars.size=" << chosenCarsRef_.size() << '\n';
    for (const auto& [id, cfg] : chosenCarsRef_) {
        std::cout << "  " << id << " -> " << cfg.carType << '\n';
    }

    player_vehicle_map_.clear();

    // barajá spawns con rng_
    if (spawns_.empty()) {
        std::cerr << "[LevelSetup] WARNING: no hay spawns.\n";
        return;
    }

    std::vector<Spawn> shuffled = spawns_;
    std::shuffle(shuffled.begin(), shuffled.end(), rng_);

    size_t i = 0;
    for (const auto& [player_id, cfg] : chosenCarsRef_) {
        if (i >= shuffled.size()) {
            std::cerr << "[LevelSetup] WARNING: faltan spawns para jugadores.\n";
            break;
        }
        const auto it = config_map_.find(cfg.carType);
        if (it == config_map_.end()) {
            std::cerr << "[LevelSetup] ERROR: no hay VehicleSpec para '" << cfg.carType << "'.\n";
            continue;
        }
        const VehicleSpec& spec = it->second;
        const Spawn& spawn = shuffled[i++];

        auto v = std::make_unique<Vehicle>(world_, spec, spawn, player_id);

        FixtureTag* tag = makeTag(vehicle_tags_, EntityKind::Vehicle, player_id);
        v->setFixtureTag(tag);
        
        player_vehicle_map_.emplace(player_id, std::move(v));
    }
}

void LevelSetup::step(float fixed_dt, int subSteps) { b2World_Step(world_, fixed_dt, subSteps); }
