#include "./resource_loader.h"

#include <cctype>
#include <iostream>

#include <yaml-cpp/yaml.h>

ResourceLoader::ResourceLoader(SDL2pp::Renderer& renderer):
        carSprites_(renderer, "../client/assets/need-for-speed/cars/cars.png") {

    LoadCarSprites();
    std::cout << "[ResourceLoader] Loaded car sprites from YAML\n";
}

void ResourceLoader::LoadCarSprites() {
    try {
        YAML::Node config = YAML::LoadFile("./client/resources/static/cars.yaml");

        YAML::Node sprites = config["sprites"];
        if (!sprites) {
            std::cerr << "[ResourceLoader] ERROR: No 'sprites' section in YAML\n";
            return;
        }

        for (auto itVeh = sprites.begin(); itVeh != sprites.end(); ++itVeh) {
            std::string vehicle = itVeh->first.as<std::string>();
            YAML::Node frames = itVeh->second["frames"];

            if (!frames) {
                std::cerr << "[ResourceLoader] WARNING: vehicle '" << vehicle
                          << "' has no frames section\n";
                continue;
            }

            for (auto itFrame = frames.begin(); itFrame != frames.end(); ++itFrame) {
                std::string frameName = itFrame->first.as<std::string>();

                auto f = itFrame->second;

                int x = f["x"].as<int>();
                int y = f["y"].as<int>();
                int w = f["w"].as<int>();
                int h = f["h"].as<int>();

                std::string spriteKey = vehicle;
                spriteKey[0] = std::toupper(spriteKey[0]);
                spriteKey += frameName;

                carSprites_.AddSprite(spriteKey, SDL2pp::Rect(x, y, w, h));
            }

            std::cout << "[ResourceLoader] Loaded frames for " << vehicle << std::endl;
        }

    } catch (const YAML::Exception& e) {
        std::cerr << "[ResourceLoader] YAML parse error: " << e.what() << std::endl;
    }
}

SpriteSheet& ResourceLoader::GetCarSprites() { return carSprites_; }
