#include "LevelAdministrator.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

bool LevelAdministrator::loadCollisionGrid(const std::string& path,
                                           const std::string& levelId,
                                           const std::string& layerId,
                                           std::string* outErr)
{
    try {
        std::ifstream in(path);
        if (!in) throw std::runtime_error("No pude abrir: " + path);
        json J; in >> J;

        // Detectar .ldtk (proyecto) vs .ldtkl (nivel)
        const json* L = nullptr;

        if (J.contains("levels") && J["levels"].is_array()) {
            // ---- Proyecto (.ldtk) ----
            const auto& levels = J["levels"];
            if (levelId.empty()) {
                if (levels.empty()) throw std::runtime_error("No hay levels en el .ldtk");
                L = &levels.at(0);
            } else {
                for (const auto& lv : levels) {
                    if (lv.value("__identifier", std::string()) == levelId) {
                        L = &lv; break;
                    }
                }
                if (!L) throw std::runtime_error("Level no encontrado: " + levelId);
            }
        } else {
            // ---- Nivel externo (.ldtkl) ----
            L = &J;
            // (opcional) validar nombre
            if (!levelId.empty()) {
                std::string id = J.value("__identifier", std::string());
                if (id != levelId)
                    throw std::runtime_error("Este .ldtkl es '" + id + "', no '" + levelId + "'");
            }
        }

        // Buscar la capa IntGrid
        if (!L->contains("layerInstances"))
            throw std::runtime_error("level.layerInstances no existe");

        for (const auto& li : L->at("layerInstances")) {
            const std::string id  = li.value("__identifier", std::string());
            const std::string typ = li.value("__type",       std::string());
            if (id == layerId && typ == "IntGrid") {
                G_.gridPx = li.at("__gridSize").get<int>();
                G_.w      = li.at("__cWid").get<int>();
                G_.h      = li.at("__cHei").get<int>();

                // Preferimos intGridCsv (denso)
                if (li.contains("intGridCsv")) {
                    const auto& csv = li.at("intGridCsv");
                    G_.v.assign(G_.w * G_.h, 0);
                    for (int i = 0; i < (int)csv.size(); ++i)
                        G_.v[i] = static_cast<uint8_t>(csv[i].get<int>());
                    return true;
                }
                // (fallback) intGrid (sparse)
                if (li.contains("intGrid") && li.at("intGrid").is_array()) {
                    G_.v.assign(G_.w * G_.h, 0);
                    for (const auto& cell : li.at("intGrid")) {
                        int cx = cell.at("coordId").get<int>() % G_.w;
                        int cy = cell.at("coordId").get<int>() / G_.w;
                        int v  = cell.at("v").get<int>();
                        if (cx>=0 && cy>=0 && cx<G_.w && cy<G_.h) G_.v[cy*G_.w + cx] = (uint8_t)v;
                    }
                    return true;
                }
                throw std::runtime_error("La capa IntGrid no tiene intGridCsv ni intGrid");
            }
        }
        throw std::runtime_error("Layer IntGrid no encontrada: " + layerId);
    } catch (const std::exception& e) {
        if (outErr) *outErr = e.what();
        return false;
    }
}

void LevelAdministrator::cellCenterMeters_(int x, int y, const BuildConfig& cfg,
                                           float cell_m, float& cx, float& cy) const
{
    // LDtk: origen arriba-izq (x→derecha, y→abajo)
    // Centro de la celda
    cx = x * cell_m + cell_m * 0.5f;

    if (!cfg.flipY) {
        cy = y * cell_m + cell_m * 0.5f;
    } else {
        // Invertir Y usando alto en px del nivel
        // Nota: para flip correcto, pasá levelPxHeight en cfg
        const float level_h_m = (float)cfg.levelPxHeight / cfg.ppm;
        cy = level_h_m - (y * cell_m + cell_m * 0.5f);
    }
}

b2BodyId LevelAdministrator::buildCollision(b2WorldId world, const BuildConfig& cfg)
{
    if (!hasGrid()) return b2_nullBodyId;
    if (cfg.mergeHorizontal) return buildMergeHorizontal_(world, cfg);
    return buildCellByCell_(world, cfg);
}

b2BodyId LevelAdministrator::buildCellByCell_(b2WorldId world, const BuildConfig& cfg)
{
    const float cell_m = (float)G_.gridPx / cfg.ppm;

    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(world, &bd);

    b2ShapeDef sd = b2DefaultShapeDef(); // en 3.x no tiene friction/restitution

    const float hx = cell_m * 0.5f;
    const float hy = cell_m * 0.5f;

    for (int y = 0; y < G_.h; ++y) {
        for (int x = 0; x < G_.w; ++x) {
            if (G_.v[y*G_.w + x] == 0) continue;

            float cx, cy;
            cellCenterMeters_(x, y, cfg, cell_m, cx, cy);

            // Polígono con centro LOCAL desplazado dentro del body
            b2Polygon poly = b2MakeOffsetBox(hx, hy, b2Vec2{cx, cy}, b2MakeRot(0.0f));

            b2ShapeId s = b2CreatePolygonShape(body, &sd, &poly);
            b2Shape_SetFriction(s,    cfg.friction);
            b2Shape_SetRestitution(s, cfg.restitution);
        }
    }
    return body;
}

b2BodyId LevelAdministrator::buildMergeHorizontal_(b2WorldId world, const BuildConfig& cfg)
{
    const float cell_m = (float)G_.gridPx / cfg.ppm;

    b2BodyDef bd = b2DefaultBodyDef();
    bd.type = b2_staticBody;
    b2BodyId body = b2CreateBody(world, &bd);

    b2ShapeDef sd = b2DefaultShapeDef();

    for (int y = 0; y < G_.h; ++y) {
        int x = 0;
        while (x < G_.w) {
            // saltar vacíos
            while (x < G_.w && G_.v[y*G_.w + x] == 0) ++x;
            if (x >= G_.w) break;

            // inicio del run
            int x0 = x;
            while (x < G_.w && G_.v[y*G_.w + x] != 0) ++x;
            int runLen = x - x0;

            // centro y tamaño del rect en metros
            float cx0, cy0, cx1, cy1;
            cellCenterMeters_(x0,  y, cfg, cell_m, cx0, cy0);
            cellCenterMeters_(x-1, y, cfg, cell_m, cx1, cy1);

            const float cx = (cx0 + cx1) * 0.5f;
            const float cy = cy0; // misma fila
            const float halfW = (runLen * cell_m) * 0.5f;
            const float halfH =  cell_m * 0.5f;

            // Box con offset local (cx,cy)
            b2Polygon poly = b2MakeOffsetBox(halfW, halfH, b2Vec2{cx, cy}, b2MakeRot(0.0f));

            b2ShapeId s = b2CreatePolygonShape(body, &sd, &poly);
            b2Shape_SetFriction(s,    cfg.friction);
            b2Shape_SetRestitution(s, cfg.restitution);
        }
    }
    return body;
}

void LevelAdministrator::debugDraw(SDL_Renderer* r,
                                   float camX_px, float camY_px,
                                   float zoom,
                                   float screen_w, float screen_h,
                                   SDL_Color color) const
{
    if (G_.v.empty()) return;

    const float cxScr = screen_w * 0.5f;
    const float cyScr = screen_h * 0.5f;

    // tamaño de una celda en pantalla (px de pantalla)
    const float cellScreen = G_.gridPx * zoom;
    const float halfCellScreen = cellScreen * 0.5f;

    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    // Recorremos la grilla; cada celda sólida -> un rect centrado
    for (int y = 0; y < G_.h; ++y) {
        for (int x = 0; x < G_.w; ++x) {
            if (G_.v[y * G_.w + x] == 0) continue;

            // centro de la celda en "px de mundo"
            const float world_cx_px = (x + 0.5f) * G_.gridPx;
            const float world_cy_px = (y + 0.5f) * G_.gridPx;

            // transformar a pantalla (misma fórmula que usás para el auto)
            const float scr_cx = (world_cx_px - camX_px) * zoom + cxScr;
            const float scr_cy =  screen_h * 0.5f - (world_cy_px - camY_px) * zoom;

            // top-left en pantalla
            SDL_FRect rect{
                scr_cx - halfCellScreen,
                scr_cy - halfCellScreen,
                cellScreen,
                cellScreen
            };

            SDL_RenderFillRectF(r, &rect);
            // Si preferís contorno:
            // SDL_RenderDrawRectF(r, &rect);
        }
    }
}