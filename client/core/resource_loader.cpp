#include "./resource_loader.h"

#include <cctype>  // para toupper
#include <iostream>

#include <yaml-cpp/yaml.h>

ResourceLoader::ResourceLoader(SDL2pp::Renderer& renderer)
        // ✅ ahora cargamos la textura real (imagen PNG)
        :
        carSprites_(renderer, "../client/assets/need-for-speed/cars/cars.png") {

    LoadCarSprites();
    std::cout << "[ResourceLoader] Loaded car sprites from YAML\n";
}

void ResourceLoader::LoadCarSprites() {
    try {
        // ✅ ruta al YAML con las posiciones
        YAML::Node config = YAML::LoadFile("../client/cars.yaml");

        YAML::Node sprites = config["sprites"];
        if (!sprites) {
            std::cerr << "[ResourceLoader] ERROR: No 'sprites' section in YAML\n";
            return;
        }

        // ✅ iterar todos los vehículos
        for (auto itVeh = sprites.begin(); itVeh != sprites.end(); ++itVeh) {
            std::string vehicle = itVeh->first.as<std::string>();  // ej. "jeep"
            YAML::Node frames = itVeh->second["frames"];

            if (!frames) {
                std::cerr << "[ResourceLoader] WARNING: vehicle '" << vehicle
                          << "' has no frames section\n";
                continue;
            }

            // ✅ recorrer cada frame del vehículo
            for (auto itFrame = frames.begin(); itFrame != frames.end(); ++itFrame) {
                std::string frameName = itFrame->first.as<std::string>();  // ej. "Sur"
                auto f = itFrame->second;

                int x = f["x"].as<int>();
                int y = f["y"].as<int>();
                int w = f["w"].as<int>();
                int h = f["h"].as<int>();

                // ✅ formatear nombre tipo "JeepSur", "JeepNorte", etc.
                std::string spriteKey = vehicle;
                spriteKey[0] = std::toupper(spriteKey[0]);  // mayúscula inicial
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
