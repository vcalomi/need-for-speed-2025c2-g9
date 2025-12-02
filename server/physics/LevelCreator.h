#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SDL2pp/SDL2pp.hh>
#include <box2d/box2d.h>

#include "../constants.h"

#include "EntityTags.h"
#include "CheckpointInfo.h"
#include "NpcInfo.h"

class LevelCreator {
public:
    using Matrix = std::vector<std::vector<int>>;
    const std::vector<Matrix>& levels() const { return levels_matrices_; }
    Matrix BuildLevelMatrix(const std::filesystem::path& file_path);
    void processDirectoryLevel(const std::string& directory_path);
    void createLevelCollision(b2WorldId world, const std::vector<Matrix>& levels);
    void createCheckpoints(b2WorldId world, const std::vector<CheckpointInfo>& input, std::vector<CheckpointInfo>& outCheckpoints);
    void createSpawns(const std::vector<Spawn>& input, std::vector<Spawn>& outSpawns);

    void drawDebugTiles(SDL_Renderer* r, float camX_px, float camY_px, float zoom);
    void drawDebugCheckpoints(SDL_Renderer* r, float camX_px, float camY_px, float zoom);
    std::vector<NpcInfo> getNpcs(){return npcs_;}
    
private:
    std::vector<Matrix> levels_matrices_{4};  // 4 slots
    std::vector<SDL_FRect> debug_tile_rects_;
    void addDebugTileRect(float cx_px, float cy_px, float size_px);

    std::deque<FixtureTag> checkpoint_tags_;
    std::deque<FixtureTag> bridge_tags_;
    std::deque<FixtureTag> npc_tags_;
    std::vector<NpcInfo> npcs_;
    std::string currentMapName_;
};
