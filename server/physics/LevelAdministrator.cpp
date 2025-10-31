#include "LevelAdministrator.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

ChunkGrid LevelAdministrator::loadChunk(const std::string& pathLdtkl,
                                        const std::string& layerId) const
{
    // 1) Abrir y parsear el .ldtkl (un level suelto)
    std::ifstream in(pathLdtkl);
    if (!in) throw std::runtime_error("No pude abrir: " + pathLdtkl);
    json L;
    try { in >> L; } catch (...) { throw std::runtime_error("JSON inválido: " + pathLdtkl); }

    ChunkGrid out; // ya trae dims fijas desde lvlcfg

    // 2) Offsets de level en el mundo LDtk (px)
    out.level_origin_px_x = L.value("worldX", 0);
    out.level_origin_px_y = L.value("worldY", 0);

    // 3) Buscar la layer IntGrid deseada
    if (!L.contains("layerInstances") || !L["layerInstances"].is_array())
        throw std::runtime_error("level.layerInstances no existe en: " + pathLdtkl);

    const json* li = nullptr;
    for (const auto& it : L["layerInstances"]) {
        if (it.value("__identifier", std::string()) == layerId &&
            it.value("__type",       std::string()) == "IntGrid")
        { li = &it; break; }
    }
    if (!li) throw std::runtime_error("Layer IntGrid no encontrada: " + layerId);

    // 4) Offset adicional de la capa (si existe)
    out.layer_offset_px_x = li->value("__pxTotalOffsetX", li->value("pxOffsetX", 0));
    out.layer_offset_px_y = li->value("__pxTotalOffsetY", li->value("pxOffsetY", 0));

    // 5) Cargar datos (preferir intGridCsv; si no, usar intGrid disperso)
    const int W = out.cols, H = out.rows;
    out.solid_mask.assign(W * H, 0);

    if (li->contains("intGridCsv")) {
        const auto& csv = (*li)["intGridCsv"];
        if ((int)csv.size() != W * H)
            throw std::runtime_error("intGridCsv size mismatch (esperado " +
                                     std::to_string(W*H) + ") en: " + pathLdtkl);
        for (int i = 0; i < W*H; ++i)
            out.solid_mask[i] = static_cast<uint8_t>(csv[i].get<int>());
    } else if (li->contains("intGrid") && (*li)["intGrid"].is_array()) {
        for (const auto& cell : (*li)["intGrid"]) {
            int coord = cell.at("coordId").get<int>();
            int v     = cell.at("v").get<int>();
            int x = coord % W;
            int y = coord / W;
            if (x>=0 && y>=0 && x<W && y<H)
                out.solid_mask[y*W + x] = (uint8_t)v;
        }
    } else {
        throw std::runtime_error("La capa no tiene intGridCsv ni intGrid en: " + pathLdtkl);
    }
    return out;
}

b2BodyId LevelAdministrator::buildStaticCollisionBody(b2WorldId world,
                                                      const ChunkGrid& G,
                                                      float ppm,
                                                      float friction,
                                                      float restitution) const
{
    if (ppm <= 0.0f) throw std::runtime_error("ppm debe ser > 0");

    // Body estático
    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(world, &bd);

    // Material para todos los runs
    b2ShapeDef sd = b2DefaultShapeDef();
    //sd.friction    = friction;
    //sd.restitution = restitution;

    const int tile_px = G.tile_size_px;
    const int cols    = G.cols;
    const int rows    = G.rows;

    const int chunk_height_px = rows * tile_px;

    // Desplazamiento absoluto del chunk (level + layer) en píxeles
    const int world_off_px_x = G.level_origin_px_x + G.layer_offset_px_x;
    const int world_off_px_y = G.level_origin_px_y + G.layer_offset_px_y;

    // Recorremos por filas y mergeamos runs contiguos sólidos
    for (int r = 0; r < rows; ++r) {
        int c = 0;
        while (c < cols) {
            // saltar vacíos
            while (c < cols && G.solid_mask[r*cols + c] == 0) ++c;
            if (c >= cols) break;

            const int start_c = c;
            while (c < cols && G.solid_mask[r*cols + c] != 0) ++c;
            const int end_c = c; // exclusivo

            const int run_cells = end_c - start_c;
            const float run_w_px = run_cells * tile_px;
            const float run_h_px = tile_px;

            // centro local (LDtk, Y-abajo)
            const float cx_local_px = start_c * tile_px + run_w_px * 0.5f;
            const float cy_local_px = r        * tile_px + run_h_px * 0.5f;

            // flip Y dentro del chunk: LDtk -> mundo físico Y-arriba
            const float cy_local_flipped_px = float(chunk_height_px) - cy_local_px;

            // a mundo (sumar offset) y a metros
            const float cx_m     = (world_off_px_x + cx_local_px)         / ppm;
            const float cy_m     = (world_off_px_y + cy_local_flipped_px) / ppm;
            const float half_w_m = (run_w_px / ppm) * 0.5f;
            const float half_h_m = (run_h_px / ppm) * 0.5f;

            b2Polygon rect = b2MakeOffsetBox(half_w_m, half_h_m, (b2Vec2){cx_m, cy_m}, b2MakeRot(0.0f));
            b2CreatePolygonShape(body, &sd, &rect);
        }
    }

    return body;
}

void LevelAdministrator::debugDrawChunkCells(SDL_Renderer* r,
                                             const ChunkGrid& G,
                                             float camX_px,
                                             float camY_px,
                                             float screen_w,
                                             float screen_h,
                                             SDL_Color color,
                                             bool fill) const
{
    if (!r || G.solid_mask.empty()) return;

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    const int cols = G.cols;
    const int rows = G.rows;
    const int tilePx = G.tile_size_px;
    const int chunk_h_px = rows * tilePx;

    const int off_x_px = G.level_origin_px_x + G.layer_offset_px_x;
    const int off_y_px = G.level_origin_px_y + G.layer_offset_px_y;

    const float cxScr = screen_w * 0.5f;
    const float cyScr = screen_h * 0.5f;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (G.solid_mask[y * cols + x] == 0) continue;

            const float cx_local_px = (x + 0.5f) * tilePx;
            const float cy_local_px = (y + 0.5f) * tilePx;

            const float cy_local_flipped_px = float(chunk_h_px) - cy_local_px;
            const float cx_world_px = off_x_px + cx_local_px;
            const float cy_world_px = off_y_px + cy_local_flipped_px;

            const float cellScreen = float(tilePx);
            const float halfCell = cellScreen * 0.5f;

            const float scr_cx = (cx_world_px - camX_px) + cxScr;
            const float scr_cy =  cyScr - (cy_world_px - camY_px);

            SDL_FRect rect{
                scr_cx - halfCell,
                scr_cy - halfCell,
                cellScreen,
                cellScreen
            };

            if (fill) SDL_RenderFillRectF(r, &rect);
            else      SDL_RenderDrawRectF(r, &rect);
        }
    }
}


void LevelAdministrator::buildSpawnPoints(const ChunkGrid& G, float ppm)
{
    if (ppm <= 0.0f) throw std::runtime_error("ppm debe ser > 0");
    spawn_points.clear();
    if (G.solid_mask.empty()) return;

    const int cols = G.cols;
    const int rows = G.rows;
    const int tilePx = G.tile_size_px;

    const int chunk_h_px = rows * tilePx;

    // offsets absolutos (si los usás en otros niveles; ahora vos los seteás en 0)
    const int off_x_px = G.level_origin_px_x + G.layer_offset_px_x;
    const int off_y_px = G.level_origin_px_y + G.layer_offset_px_y;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const int idx = r * cols + c;
            const int tileId = static_cast<int>(G.solid_mask[idx]);

            // Sólo spawns
            if (tileId != 3 && tileId != 4 && tileId != 5 && tileId != 6) continue;

            // centro local de la celda (LDtk: Y hacia abajo)
            const float cx_local_px = (c + 0.5f) * tilePx;
            const float cy_local_px = (r + 0.5f) * tilePx;

            // flip Y dentro del chunk para pasar a Y-arriba (como en Box2D)
            const float cy_local_flipped_px = float(chunk_h_px) - cy_local_px;

            // a mundo en px (sumo offsets level+layer)
            const float x_world_px = off_x_px + cx_local_px;
            const float y_world_px = off_y_px + cy_local_flipped_px;

            // a metros
            const float x_m = x_world_px / ppm;
            const float y_m = y_world_px / ppm;

            spawn_points.push_back(Spawn{
                x_m,
                y_m,
                tileIdToAngle(tileId)
            });
        }
    }
}
