#pragma once

#include <box2d/box2d.h>
#include <string>
#include <vector>
#include <cstdint>
#include <SDL.h>
#include "../spawn.h"

namespace lvlcfg {
    inline constexpr int kTileSizePx     = 2;    
    inline constexpr int kChunkCols      = 192;  
    inline constexpr int kChunkRows      = 192;  
}

struct ChunkGrid {
    // Dimensiones fijas
    int tile_size_px = lvlcfg::kTileSizePx;
    int cols         = lvlcfg::kChunkCols;
    int rows         = lvlcfg::kChunkRows;

    // Origen top-left del level dentro del mundo LDtk (en píxeles)
    int level_origin_px_x = 0;  // level["worldX"]
    int level_origin_px_y = 0;  // level["worldY"]

    int layer_offset_px_x = 0;  
    int layer_offset_px_y = 0;  

    std::vector<uint8_t> solid_mask;
};

class LevelAdministrator {
    public:
        ChunkGrid loadChunk(const std::string& path_ldtkl, const std::string& layerId) const;

        b2BodyId buildStaticCollisionBody(b2WorldId world, const ChunkGrid& chunk, float ppm, float friction, float restitution) const;

        void debugDrawChunkCells(SDL_Renderer* r,
                            const ChunkGrid& G,
                            float camX_px,
                            float camY_px,
                            float screen_w,
                            float screen_h,
                            SDL_Color color = {255, 255, 0, 200},
                            bool fill = true) const;

        void buildSpawnPoints(const ChunkGrid& chunk, float ppm);
        const std::vector<Spawn>& getSpawnPoints() const { return spawn_points; }

    private:
        static inline float tileIdToAngle(int tileId) {
            switch (tileId) {
                case 3: return  +3.14159265358979323846f * 0.5f;  // arriba (π/2)
                case 2: return  0.0f;                             // derecha
                case 4: return -3.14159265358979323846f * 0.5f;   // abajo (-π/2)
                case 6: return  3.14159265358979323846f;          // izquierda (π)
                default: return 0.0f;
            }
        }

        std::vector<Spawn> spawn_points;
};  
