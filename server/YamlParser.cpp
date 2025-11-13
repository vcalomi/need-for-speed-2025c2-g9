#include "YamlParser.h"

#include <iostream>

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
