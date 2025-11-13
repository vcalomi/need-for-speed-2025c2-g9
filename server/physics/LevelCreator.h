#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <box2d/box2d.h>
#include "../constants.h"
#include "spawn.h"
#include "EntityTags.h"

struct CheckpointInfo {
    float x_px;
    float y_px;
    int index;
};

class LevelCreator {
public:
    using Matrix = std::vector<std::vector<int>>;
    const std::vector<Matrix>& levels() const { return levels_matrices_; }
    Matrix BuildLevelMatrix(const std::filesystem::path& file_path);
    void processDirectoryLevel(const std::string& directory_path);
    void createLevelCollision(b2WorldId world, const std::vector<Matrix>& levels);


    void drawDebugTiles(SDL_Renderer* r, float camX_px, float camY_px, float zoom);
    const std::vector<Spawn>& getSpawnPoints() const { return spawn_points; }
    void drawDebugCheckpoints(SDL_Renderer* r, float camX_px, float camY_px, float zoom);
    const std::vector<CheckpointInfo>& get_checkpoints() const  { return checkpoints_;}
private:
    std::vector<Matrix> levels_matrices_{4};  // 4 slots
    std::vector<SDL_FRect> debug_tile_rects_;
    std::vector<Spawn> spawn_points;
    void addDebugTileRect(float cx_px, float cy_px, float size_px);

    std::deque<FixtureTag> checkpoint_tags_;

    std::vector<CheckpointInfo> checkpoints_;
};
