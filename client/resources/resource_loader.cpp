#include "./resource_loader.h"

#include <iostream>
#include <filesystem>

#include <yaml-cpp/yaml.h>

ResourceLoader::ResourceLoader(SDL2pp::Renderer& renderer): renderer_(renderer) {
    LoadCarSprites();
    LoadNpcSprites();
}

void ResourceLoader::LoadCarSprites() {
    try {
        YAML::Node config;
        try {
            config = YAML::LoadFile(std::string(RESOURCES_DIR) + "/static/cars.yaml");
        } catch (const YAML::Exception&) {
            config = YAML::LoadFile(std::string(PROJECT_SOURCE_DIR) + "/client/resources/static/cars.yaml");
        }

        YAML::Node sprites = config["sprites"];
        if (!sprites) {
            std::cerr << "[ResourceLoader] ERROR: No 'sprites' section in YAML\n";
            return;
        }

        for (auto itVeh = sprites.begin(); itVeh != sprites.end(); ++itVeh) {

            std::string vehicle = itVeh->first.as<std::string>();
            YAML::Node vehNode = itVeh->second;

            if (!vehNode["sheet_path"]) {
                std::cerr << "[ResourceLoader] WARNING: vehicle '" << vehicle
                          << "' missing sheet_path\n";
                continue;
            }

            std::string sheetPath = vehNode["sheet_path"].as<std::string>();

            auto texture = std::make_shared<SDL2pp::Texture>(renderer_, SDL2pp::Surface(sheetPath));

            YAML::Node base = vehNode["frames"]["base"];
            if (!base) {
                std::cerr << "[ResourceLoader] WARNING: vehicle '" << vehicle
                          << "' missing base frame\n";
                continue;
            }

            int x = base["x"].as<int>();
            int y = base["y"].as<int>();
            int w = base["w"].as<int>();
            int h = base["h"].as<int>();

            std::string spriteName = vehicle + "_Base";

            carSprites_.AddSprite(spriteName, texture, SDL2pp::Rect(x, y, w, h));
        }

    } catch (const YAML::Exception& e) {
        std::cerr << "[ResourceLoader] YAML parser error: " << e.what() << std::endl;
    }
}

void ResourceLoader::LoadNpcSprites() {
    try {
        YAML::Node config;
        try {
            config = YAML::LoadFile(std::string(RESOURCES_DIR) + "/static/cars.yaml");
        } catch (const YAML::Exception&) {
            config = YAML::LoadFile(std::string(PROJECT_SOURCE_DIR) + "/client/resources/static/cars.yaml");
        }

        YAML::Node npcs = config["npcs"];
        if (!npcs) {
            return;
        }

        for (const auto& npcNode: npcs) {
            if (!npcNode["id"] || !npcNode["sheet_path"]) {
                std::cerr << "[ResourceLoader] NPC missing id or sheet_path\n";
                continue;
            }

            std::string id = npcNode["id"].as<std::string>();
            std::string sheetPath = npcNode["sheet_path"].as<std::string>();

            std::string resolved = sheetPath;
            try {
                if (!std::filesystem::exists(resolved)) {
                    const auto filename = std::filesystem::path(sheetPath).filename().string();
                    const std::string installed = std::string(ASSETS_DIR) + "/people/" + filename;
                    if (std::filesystem::exists(installed)) {
                        resolved = installed;
                    } else {
                        const std::string source = std::string(PROJECT_SOURCE_DIR) + "/client/assets/need-for-speed/people/" + filename;
                        resolved = source;
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "[ResourceLoader] Error resolving NPC path '" << sheetPath
                          << "': " << e.what() << "\n";
            }

            if (!std::filesystem::exists(resolved)) {
                std::cerr << "[ResourceLoader] NPC texture not found: " << resolved << " (from "
                          << sheetPath << ")\n";
                continue;
            }

            auto texture = std::make_shared<SDL2pp::Texture>(renderer_, SDL2pp::Surface(resolved));

            int w = texture->GetWidth();
            int h = texture->GetHeight();


            npcSprites_.AddSprite(id, texture, SDL2pp::Rect(0, 0, w, h));
        }

    } catch (const YAML::Exception& e) {
        std::cerr << "[ResourceLoader] YAML parser error: " << e.what() << std::endl;
    }
}


SpriteSheet& ResourceLoader::GetCarSprites() { return carSprites_; }
SpriteSheet& ResourceLoader::GetNpcSprites() { return npcSprites_; }
