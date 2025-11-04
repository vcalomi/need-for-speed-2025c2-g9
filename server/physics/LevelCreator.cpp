#include "LevelCreator.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <system_error>
#include <utility>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using Matrix = LevelCreator::Matrix;
using Matrix_vector = std::vector<std::vector<int>>;

constexpr float PPM = 50.0f;     // píxeles por metro
constexpr int TILE_SIZE_PX = 2;  // cada tile mide 2 píxeles en LDtk


float tileIdToAngle(int tileId) {
    switch (tileId) {
        case 6:
            return +3.14159265358979323846f * 0.5f;  // west
        case 4:
            return 0.0f;  // south
        case 5:
            return -3.14159265358979323846f * 0.5f;  // east
        case 3:
            return 3.14159265358979323846f;  // north
        default:
            return 0.0f;
    }
}


void LevelCreator::addDebugTileRect(float cx_px, float cy_px, float size_px) {
    SDL_FRect r{cx_px - size_px * 0.5f, cy_px - size_px * 0.5f, size_px, size_px};
    debug_tile_rects_.push_back(r);
}

void LevelCreator::drawDebugTiles(SDL_Renderer* r, float camX_px, float camY_px, float zoom) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 255, 0, 220);
    for (const auto& aabb: debug_tile_rects_) {
        SDL_FRect s{(aabb.x - camX_px) * zoom, (aabb.y - camY_px) * zoom, aabb.w * zoom,
                    aabb.h * zoom};
        SDL_RenderDrawRectF(r, &s);
    }
}

static std::optional<int> extract_suffix_index(const std::filesystem::path& p) {
    std::string stem = p.stem().string();  // ej: "sanAndreas_2"
    auto pos = stem.find_last_of('_');
    if (pos == std::string::npos || pos + 1 >= stem.size())
        return std::nullopt;
    std::string suf = stem.substr(pos + 1);  // "2"
    for (unsigned char c: suf)
        if (!std::isdigit(c))
            return std::nullopt;
    try {
        return std::stoi(suf);
    } catch (...) {
        return std::nullopt;
    }
}

Matrix LevelCreator::BuildLevelMatrix(const std::filesystem::path& file_path) {
    std::ifstream in(file_path);
    if (!in) {
        std::cerr << "[ERROR] cannot open " << file_path << '\n';
        return {};
    }

    json J;
    try {
        in >> J;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] invalid JSON in " << file_path << ": " << e.what() << '\n';
        return {};
    }

    if (!J.contains("layerInstances") || !J["layerInstances"].is_array()) {
        std::cerr << "[ERROR] missing 'layerInstances' in " << file_path << '\n';
        return {};
    }

    const json* L = nullptr;
    for (const auto& layer: J["layerInstances"]) {
        if (!layer.is_object())
            continue;
        if (layer.value("__type", "") != "IntGrid")
            continue;
        if (layer.value("__identifier", "") == "Collision2px") {
            L = &layer;
            break;
        }
    }
    if (!L) {
        std::cerr << "[ERROR] 'Collision2px' not found in " << file_path << '\n';
        return {};
    }

    const int cw = L->value("__cWid", 0);
    const int ch = L->value("__cHei", 0);
    if (cw <= 0 || ch <= 0) {
        std::cerr << "[ERROR] invalid grid dims in " << file_path << '\n';
        return {};
    }

    if (!L->contains("intGridCsv") || !(*L)["intGridCsv"].is_array()) {
        std::cerr << "[ERROR] missing 'intGridCsv' in " << file_path << '\n';
        return {};
    }

    const auto& csv = (*L)["intGridCsv"];
    Matrix M(ch, std::vector<int>(cw, 0));
    const int total = std::min<int>(csv.size(), cw * ch);
    for (int idx = 0; idx < total; ++idx) {
        int y = idx / cw, x = idx % cw;
        M[y][x] = csv[idx].get<int>();
    }
    return M;
}

void LevelCreator::processDirectoryLevel(const std::string& directory_path) {
    std::error_code ec;
    if (!std::filesystem::exists(directory_path, ec) ||
        !std::filesystem::is_directory(directory_path, ec)) {
        std::cerr << "[ERROR] invalid directory " << directory_path << '\n';
        return;
    }

    for (const auto& entry: std::filesystem::directory_iterator(directory_path, ec)) {
        if (ec) {
            std::cerr << "[ERROR] iterating " << directory_path << ": " << ec.message() << '\n';
            break;
        }
        if (!entry.is_regular_file() || entry.path().extension() != ".ldtkl")
            continue;

        auto idxOpt = extract_suffix_index(entry.path());
        if (!idxOpt)
            continue;  // sin sufijo numérico => salteamos

        int idx = *idxOpt;
        if (idx < 0 || idx >= 4)
            continue;  // fuera del rango 0..3 => salteamos

        Matrix m = BuildLevelMatrix(entry.path());
        if (m.empty())
            continue;

        levels_matrices_[idx] = std::move(m);
        std::cerr << "[INFO] loaded " << entry.path().filename().string()
                  << " into levels_matrices_[" << idx << "]\n";
    }
}

static void createTileCollider(b2WorldId world, float x_px, float y_px, float size_px) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = {x_px / PPM, y_px / PPM};  // pasar a metros

    b2BodyId body = b2CreateBody(world, &bodyDef);

    b2Polygon box = b2MakeBox(size_px / 2.0f / PPM, size_px / 2.0f / PPM);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(body, &shapeDef, &box);
}

void LevelCreator::createLevelCollision(b2WorldId world, const std::vector<Matrix>& levels) {
    const int tile_px = TILE_SIZE_PX;
    const int level_px = 192 * tile_px;

    for (int lvl = 0; lvl < 4; ++lvl) {
        if (levels[lvl].empty())
            continue;

        int offset_x_px = (lvl % 2) * level_px;
        int offset_y_px = (lvl / 2) * level_px;

        const Matrix_vector& M = levels[lvl];
        int h = M.size();
        int w = M[0].size();

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int v = M[y][x];
                if (v == 0)
                    continue;  // sin colisión

                float world_x_px = offset_x_px + x * tile_px + tile_px / 2.0f;
                float world_y_px = offset_y_px + y * tile_px + tile_px / 2.0f;

                if (v == 3 || v == 4 || v == 5 || v == 6) {
                    spawn_points.push_back(
                            Spawn{world_x_px / PPM, world_y_px / PPM, tileIdToAngle(v)});
                    continue;
                }

                createTileCollider(world, world_x_px, world_y_px, tile_px);
                addDebugTileRect(world_x_px, world_y_px, tile_px);
            }
        }
    }

    std::cout << "[INFO] colisiones creadas\n";
}
