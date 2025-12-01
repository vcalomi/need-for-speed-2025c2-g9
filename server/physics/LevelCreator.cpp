#include "LevelCreator.h"

#include <algorithm>
#include <cctype>
#include <optional>
#include <system_error>
#include <utility>

#include <nlohmann/json.hpp>


using json = nlohmann::json;
using Matrix = LevelCreator::Matrix;
using Matrix_vector = std::vector<std::vector<int>>;


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
            continue;

        int idx = *idxOpt;
        if (idx < 0 || idx >= 4)
            continue; 

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
    bodyDef.position = {x_px / PPM, y_px / PPM};

    b2BodyId body = b2CreateBody(world, &bodyDef);

    b2Polygon box = b2MakeBox(size_px / 2.0f / PPM, size_px / 2.0f / PPM);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(body, &shapeDef, &box);
}

void LevelCreator::createLevelCollision(b2WorldId world, const std::vector<Matrix>& levels) {
    const int tile_px = TILE_SIZE_PX; // 2 px

    if (levels.empty()) {
        std::cerr << "[ERROR] createLevelCollision: levels vacío\n";
        return;
    }

    auto mutableLevels = levels;

    for (int lvl = 0; lvl < 4; ++lvl) {
        if (lvl >= (int)levels.size())
            continue;

        Matrix& M =  mutableLevels[lvl];
        if (M.empty())
            continue;

        const int h = (int)M.size();
        const int w = (int)M[0].size();
        if (w == 0)
            continue;

        const int chunk_w_px = w * tile_px; 
        const int chunk_h_px = h * tile_px;

        const int offset_x_px = (lvl % 2) * chunk_w_px;
        const int offset_y_px = (lvl / 2) * chunk_h_px;

        for (int y = 0; y < h; ++y) {
            if ((int)M[y].size() != w)
                continue;

            for (int x = 0; x < w; ++x) {
                int v = M[y][x];
                if (v == 0)
                    continue; // sin colisión

                const float world_x_px =
                    offset_x_px + x * tile_px + tile_px * 0.5f;
                const float world_y_px =
                    offset_y_px + y * tile_px + tile_px * 0.5f;

                if (v == 3 || v == 4 || v == 5 || v == 6) {
                    spawn_points.push_back(
                        Spawn{world_x_px / PPM, world_y_px / PPM, tileIdToAngle(v)});
                    continue;
                }

                if (v == 18 || v == 19) {
                    int xStart = x;
                    int xEnd   = x;

                    // greedy: agrandamos hacia la derecha mientras sea el mismo tile
                    while (xEnd + 1 < w && M[y][xEnd + 1] == v) {
                        ++xEnd;
                    }

                    const int numTiles = (xEnd - xStart + 1);

                    const float center_x_px =
                        offset_x_px + ((xStart + xEnd + 1) * 0.5f) * tile_px;
                    const float center_y_px = world_y_px;

                    const float halfWidth_m  = (numTiles * tile_px) * 0.5f / PPM;
                    const float halfHeight_m = tile_px * 0.5f / PPM;

                    const float cx_m = center_x_px / PPM;
                    const float cy_m = center_y_px / PPM;

                    b2BodyDef bodyDef = b2DefaultBodyDef();
                    bodyDef.type = b2_staticBody;
                    bodyDef.position = {cx_m, cy_m};
                    b2BodyId body = b2CreateBody(world, &bodyDef);

                    b2Polygon box = b2MakeBox(halfWidth_m, halfHeight_m);

                    b2ShapeDef shapeDef = b2DefaultShapeDef();
                    shapeDef.isSensor = true;
                    shapeDef.enableSensorEvents = true;

                    // data: 1 = down (18), 0 = up (19)
                    int data = (v == 18) ? 1 : 0;
                    FixtureTag* tag =
                        makeTag(bridge_tags_, EntityKind::BridgeToggle, 0 ,data);
                    shapeDef.userData = tag;

                    b2CreatePolygonShape(body, &shapeDef, &box);

                    // pisar los tiles que ya procesamos con 0
                    for (int k = xStart; k <= xEnd; ++k) {
                        M[y][k] = 0;
                    }

                    continue;
                }

                if (v >= 7) {
                    const int checkpointIndex = v - 7;  // 7->0, 8->1, etc.

                    const float cx_m = world_x_px / PPM;
                    const float cy_m = world_y_px / PPM;
                    const float radius_m = CHECKPOINT_RADIUS_PX / PPM;   

                    b2BodyDef bodyDef = b2DefaultBodyDef();
                    bodyDef.type = b2_staticBody;
                    bodyDef.position = {cx_m, cy_m};
                    b2BodyId body = b2CreateBody(world, &bodyDef);


                    b2Circle circle{};
                    circle.center = {0.0f, 0.0f};
                    circle.radius = radius_m;

                    b2ShapeDef shapeDef = b2DefaultShapeDef();
                    shapeDef.isSensor = true;
                    shapeDef.enableSensorEvents = true; 
                    
                    FixtureTag* tag = makeTag(checkpoint_tags_, EntityKind::Checkpoint, checkpointIndex, 0);
                    shapeDef.userData = tag;
                    b2CreateCircleShape(body, &shapeDef, &circle);

                    checkpoints_.push_back(CheckpointInfo{world_x_px, world_y_px, checkpointIndex});

                    continue; 
                }

                createTileCollider(world, world_x_px, world_y_px, tile_px);
                addDebugTileRect(world_x_px, world_y_px, tile_px);
            }
        }
    }

    std::cout << "[INFO] colisiones creadas\n";
}

void LevelCreator::drawDebugCheckpoints(SDL_Renderer* r,
                                        float camX_px,
                                        float camY_px,
                                        float zoom)
{
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(r, 0, 0, 255, 200);  // azul semitransparente

    const int SEGMENTS = 24;
    const float radius_px = CHECKPOINT_RADIUS_PX;  

    for (const auto& cp : checkpoints_) {
        const float cx_px = cp.x_px;
        const float cy_px = cp.y_px;
        const float cx = (cx_px - camX_px) * zoom;
        const float cy = (cy_px - camY_px) * zoom;
        const float rad = radius_px * zoom;

        float prev_x = cx + rad;
        float prev_y = cy;

        for (int i = 1; i <= SEGMENTS; ++i) {
            float theta = (2.0f * 3.14159265358979323846f * i) / SEGMENTS;
            float x = cx + rad * std::cos(theta);
            float y = cy + rad * std::sin(theta);
            SDL_RenderDrawLineF(r, prev_x, prev_y, x, y);
            prev_x = x;
            prev_y = y;
        }

        // opcional: número del checkpoint
        SDL_SetRenderDrawColor(r, 255, 255, 255, 220);
        SDL_FRect rect = {cx - 2, cy - 2, 4, 4};
        SDL_RenderFillRectF(r, &rect);
    }
}