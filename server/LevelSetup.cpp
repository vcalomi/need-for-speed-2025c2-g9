#include "LevelSetup.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "./physics/EntityTags.h"

LevelSetup::LevelSetup(const std::string& level_json_path, const std::string& vehicle_specs_path,
                       const std::map<int, CarConfig>& chosenCars, std::unordered_map<int, CarUpgrades>& upgradesByUser):
        chosenCarsRef_(chosenCars), config_map_(YamlParser{}.parse(vehicle_specs_path)), upgradesByUserRef_(upgradesByUser) {

    // Create empty world
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    world_ = b2CreateWorld(&wdef);

    // Create level hitboxes
    levelCreator_.processDirectoryLevel(level_json_path);
    levelCreator_.createLevelCollision(world_, levelCreator_.levels());

    spawns_ = levelCreator_.getSpawnPoints();
    checkpoints_ = levelCreator_.get_checkpoints();
    buildVehicles();
}

void LevelSetup::buildVehicles() {
    std::cout << "[LevelSetup] chosenCars.size=" << chosenCarsRef_.size() << '\n';
    for (const auto& [id, cfg]: chosenCarsRef_) {
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
    for (const auto& [player_id, cfg]: chosenCarsRef_) {
        if (i >= shuffled.size()) {
            std::cerr << "[LevelSetup] WARNING: faltan spawns para jugadores.\n";
            break;
        }
        const auto it = config_map_.find(cfg.carType);
        if (it == config_map_.end()) {
            std::cerr << "[LevelSetup] ERROR: no hay VehicleSpec para '" << cfg.carType << "'.\n";
            continue;
        }

        VehicleSpec spec = it->second;

        auto itUp = upgradesByUserRef_.find(player_id);
        if (itUp != upgradesByUserRef_.end()) {
            applyUpgradesToSpec(spec, itUp->second);
        }
        const Spawn& spawn = shuffled[i++];

        FixtureTag* tag = makeTag(vehicle_tags_, EntityKind::Vehicle, player_id);

        auto v = std::make_unique<Vehicle>(world_, spec, spawn, player_id, tag);

        player_vehicle_map_.emplace(player_id, std::move(v));
    }
}

void LevelSetup::applyUpgradesToSpec(VehicleSpec& spec, const CarUpgrades& up) {
    // +10% por nivel
    spec.engine_force_N *= (1.0f + 0.10f * up.engineLevel);
    spec.health *= (1.0f + 0.10f * up.armorLevel);
}


std::vector<RawEvent> LevelSetup::stepAndDrainEvents(float dt) {
    collector_.capturePreStepSpeeds(player_vehicle_map_);
    const float substepDt = dt;
    const int substeps = 4;
    b2World_Step(world_, substepDt, substeps);

    collector_.collect(world_);

    return collector_.drain();  
}

float LevelSetup::getVehicleSpeed(int vehicle_id) {
    const auto& speeds = collector_.getLastSpeeds();
    auto it = speeds.find(vehicle_id);
    if (it != speeds.end()) {
        return it->second;
    }
    return 0.0f;
}
