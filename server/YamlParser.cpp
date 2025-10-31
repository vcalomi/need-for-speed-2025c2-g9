#include "./YamlParser.h"

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

            VehicleSpec vehicle{v["width_m"].as<double>(),       v["height_m"].as<double>(),
                                v["density"].as<double>(),       v["engine_force_N"].as<double>(),
                                v["brake_force_N"].as<double>(), v["steer_torque"].as<double>(),
                                v["friction"].as<double>(),      v["restitution"].as<double>()};

            vehicles[name] = vehicle;
        }

        // Ejemplo de uso
        /*for (const auto& [name, v]: vehicles) {
            std::cout << "Vehicle: " << name << "\n";
            std::cout << "  Width: " << v.width_m << " m\n";
            std::cout << "  Height: " << v.height_m << " m\n";
            std::cout << "  Density: " << v.density << " kg/m^3\n";
            std::cout << "  Engine force: " << v.engine_force_N << " N\n";
            std::cout << "  Brake force: " << v.brake_force_N << " N\n";
            std::cout << "  Steer torque: " << v.steer_torque << "\n";
            std::cout << "  Friction: " << v.friction << "\n";
            std::cout << "  Restitution: " << v.restitution << "\n\n";
        }*/
        return vehicles;

    } catch (const YAML::Exception& e) {
        std::cerr << "Error parsing YAML: " << e.what() << std::endl;
        return {};
    }
}