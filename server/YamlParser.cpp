#include "YamlParser.h"

#include <iostream>
#include <utility>

#include <yaml-cpp/yaml.h>


std::unordered_map<std::string, VehicleSpec> YamlParser::parse(const std::string& filename) {
    try {
        YAML::Node config = YAML::LoadFile(filename);

        std::unordered_map<std::string, VehicleSpec> vehicles;

        YAML::Node vehiclesNode = config["vehicles"];
        for (auto it = vehiclesNode.begin(); it != vehiclesNode.end(); ++it) {
            std::string name = it->first.as<std::string>();
            YAML::Node v = it->second;

            VehicleSpec vehicle{v["width_m"].as<float>(),       v["height_m"].as<float>(),
                                v["density"].as<float>(),       v["engine_force_N"].as<float>(),
                                v["brake_force_N"].as<float>(), v["steer_torque"].as<float>(),
                                v["friction"].as<float>(),      v["restitution"].as<float>(),
                                v["health_points"].as<float>()};

            vehicles[name] = vehicle;
        }

        return vehicles;

    } catch (const YAML::Exception& e) {
        std::cerr << "Error parsing YAML: " << e.what() << std::endl;
        return {};
    }
}

RaceInfo YamlParser::parseRaceInfo(const std::string& filename) {
    RaceInfo info;

    YAML::Node root;
    try {
        root = YAML::LoadFile(filename);
    } catch (const std::exception& e) {
        std::cerr << "[YamlParser] Error reading YAML: " << e.what() << "\n";
        return info;
    }

    if (root["map_image"])
        info.mapName = root["map_image"].as<std::string>();
    else if (root["city_id"])
        info.mapName = root["city_id"].as<std::string>();
    else
        info.mapName = "unknown";


    std::vector<CheckpointInfo> cps;

    auto loadCP = [&](const YAML::Node& pos, int index) {
        if (!pos || pos.size() != 2)
            return;

        float nx = pos[0].as<float>();  // normalized [0–1]
        float ny = pos[1].as<float>();

        cps.push_back({nx *2320 , ny * 2336, index});
    };

    int index = 0;

    if (root["start"]) {
        loadCP(root["start"]["position"], index++);
    }

    if (root["checkpoints"]) {
        for (const auto& cp: root["checkpoints"]) {
            loadCP(cp["position"], index++);
        }
    }

    if (root["finish"]) {
        loadCP(root["finish"]["position"], index++);
    }

    info.checkpoints = std::move(cps);

    if (root["player_spawns"]) {
        for (const auto& sp: root["player_spawns"]) {
            const YAML::Node& pos = sp["position"];
            if (!pos || pos.size() != 2)
                continue;

            Spawn s;
            s.x = pos[0].as<float>() * 2320;
            s.y = pos[1].as<float>() * 2336;
            s.angle = sp["angle"] ? sp["angle"].as<float>() : 0.0f;


            info.spawns.push_back(s);
        }
    }
    return info;
}
