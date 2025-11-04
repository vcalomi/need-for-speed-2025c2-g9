#include "LevelSetup.h"




LevelSetup::LevelSetup(const std::string& level_json_path, const std::string& vehicle_specs_path, std::map<int, CarConfig>& chosenCars): 
config_map_(YamlParser{}.parse(vehicle_specs_path)){

    //Create empty world
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    world_ = b2CreateWorld(&wdef);

    //Create level hitboxes
    levelCreator_.processDirectoryLevel(level_json_path);
    levelCreator_.createLevelCollision(world_, levelCreator_.levels());
    
    spawns_ = levelCreator_.getSpawnPoints();
    buildVehicles(chosenCars);

}

void LevelSetup::buildVehicles(const std::map<int, CarConfig>& chosenCars) {
    player_vehicle_map_.clear();

    std::vector<Spawn> shuffled = spawns_;
    std::shuffle(shuffled.begin(), shuffled.end(), rng_);

    size_t i = 0;
    for (const auto& [player_id, cfg] : chosenCars) {
        const auto& spec = config_map_.at(cfg.carType);  
        const Spawn& spawn = shuffled[i++];
        auto v = std::make_unique<Vehicle>(world_, spec, spawn, player_id);
        player_vehicle_map_.emplace(player_id, std::move(v));
    }
}

void LevelSetup::step(float fixed_dt, int subSteps) {
    b2World_Step(world_, fixed_dt, subSteps);
}