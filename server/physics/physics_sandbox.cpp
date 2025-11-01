#include <box2d/box2d.h>
#include <SDL.h>
#include <iostream>
#include "../YamlParser.h"
#include "../vehicle.h"
#include "../spawn.h"
#include "../constants.h"
#include "LevelAdministrator.h"
#include <algorithm>
#include <vector>


inline constexpr float PPM = 2.0f;     

int main() {
    // --- Config ---
    YamlParser parser;
    auto mapa_config = parser.parse("../server/vehicle_specs.yaml");
    VehicleSpec ferrari_spec = mapa_config["ferrari_F40"];

    // --- SDL ---
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Physics Sandbox",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1200, 800, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    const float screen_w = 1200.0f;
    const float screen_h = 800.0f;

    // --- Cámara ---
    float camX_px = 0.0f, camY_px = -200.0f; // cámara en píxeles de mundo
    const float panSpeed_px = 400.0f;        // velocidad de paneo en px/seg

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;
    const double freq = (double)SDL_GetPerformanceFrequency();

    // --- Mundo Box2D (top-down) ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId worldId = b2CreateWorld(&wdef);

    // --- Nivel LDtk ---
    LevelAdministrator admin;                               
    ChunkGrid chunk = admin.loadChunk("../server/physics/Level_0.ldtkl", "Collision2px");
    ChunkGrid chunk_2 = admin.loadChunk("../server/physics/Level_1.ldtkl", "Collision2px");
    ChunkGrid chunk_3 = admin.loadChunk("../server/physics/Level_2.ldtkl", "Collision2px");
    ChunkGrid chunk_4 = admin.loadChunk("../server/physics/Level_3.ldtkl", "Collision2px");

    b2BodyId mapBody = admin.buildStaticCollisionBody(worldId, chunk, PPM, /*friction=*/0.8f, /*rest=*/0.0f);
    b2BodyId mapBody_2 = admin.buildStaticCollisionBody(worldId, chunk_2, PPM, /*friction=*/0.8f, /*rest=*/0.0f);
    b2BodyId mapBody_3 = admin.buildStaticCollisionBody(worldId, chunk_3, PPM, /*friction=*/0.8f, /*rest=*/0.0f);
    b2BodyId mapBody_4 = admin.buildStaticCollisionBody(worldId, chunk_4, PPM, /*friction=*/0.8f, /*rest=*/0.0f);
    
    std::cout << "solid tiles: "
              << std::count_if(chunk.solid_mask.begin(), chunk.solid_mask.end(),
                               [](uint8_t v){ return v != 0; })
              << "\n";
        std::cout << "solid tiles2: "
              << std::count_if(chunk_2.solid_mask.begin(), chunk_2.solid_mask.end(),
                               [](uint8_t v){ return v != 0; })
              << "\n";
    std::cout << "solid tiles3: "
              << std::count_if(chunk_3.solid_mask.begin(), chunk_3.solid_mask.end(),
                               [](uint8_t v){ return v != 0; })
              << "\n";
    std::cout << "solid tiles4: "
              << std::count_if(chunk_4.solid_mask.begin(), chunk_4.solid_mask.end(),
                               [](uint8_t v){ return v != 0; })
              << "\n";
    admin.buildSpawnPoints(chunk, PPM);
    std::vector<Spawn> vect_spawn = admin.getSpawnPoints();

    // --- Auto ---
    Vehicle vehicle(worldId, ferrari_spec,vect_spawn[0], /*player_id=*/0);

    bool running = true;
    SDL_Event e;

    while (running) {
        // dt para pan suave
        last = now; now = SDL_GetPerformanceCounter();
        float dt = float((now - last) / freq);

        // --- Input ---
        TurnDir dir = TurnDir::None;
        bool go = false, stop = false;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        const Uint8* ks = SDL_GetKeyboardState(NULL);
        go   = ks[SDL_SCANCODE_W] || ks[SDL_SCANCODE_UP];
        stop = ks[SDL_SCANCODE_S] || ks[SDL_SCANCODE_DOWN];
        if (ks[SDL_SCANCODE_A] || ks[SDL_SCANCODE_LEFT])  dir = TurnDir::Left;
        if (ks[SDL_SCANCODE_D] || ks[SDL_SCANCODE_RIGHT]) dir = TurnDir::Right;
        if (go)   vehicle.accelerate();
        if (stop) vehicle.brake();
        vehicle.turn(dir);

        // --- Step física ---
        b2World_Step(worldId, 1.0f / 60.0f, 8);

        // --- Pose (metros) ---
        float x_m, y_m, ang_rad;
        vehicle.getPosition(x_m, y_m, ang_rad);

        // --- Cámara que sigue al auto ---
        float world_x_px = x_m * PPM;
        float world_y_px = y_m * PPM;
        float lerp = 5.0f * dt;
        camX_px += (world_x_px - camX_px) * lerp;
        camY_px += (world_y_px - camY_px) * lerp;

        // --- Transformaciones a pantalla ---
        float x_px = (world_x_px - camX_px) + screen_w * 0.5f;
        float y_px =  screen_h * 0.5f - (world_y_px - camY_px);

        // --- Rotación del auto ---
        float a = -ang_rad; 
        float c = cosf(a), s = sinf(a);

        // Tamaños: (m -> px)
        float w_px = vehicle.width()  * PPM;
        float h_px = vehicle.height() * PPM;

        const float hw = w_px * 0.5f;
        const float hh = h_px * 0.5f;

        SDL_FPoint local[4] = { {-hw,-hh},{hw,-hh},{hw,hh},{-hw,hh} };

        SDL_Vertex verts[4];
        for (int i = 0; i < 4; ++i) {
            float rx = local[i].x * c - local[i].y * s;
            float ry = local[i].x * s + local[i].y * c;
            verts[i].position.x = x_px + rx;
            verts[i].position.y = y_px + ry;
            verts[i].color = {0, 255, 0, 255};
            verts[i].tex_coord = {0, 0};
        }

        int indices[6] = {0,1,2, 0,2,3};

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Auto
        SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);

        // Mapa (sin zoom)
        admin.debugDrawChunkCells(renderer,
                                  chunk,
                                  camX_px, camY_px,
                                  screen_w, screen_h,
                                  {255, 255, 0, 160},
                                  false);
        admin.debugDrawChunkCells(renderer,
                                chunk_2,
                                  camX_px, camY_px,
                                  screen_w, screen_h,
                                  {255, 255, 0, 160},
                                  false);
        admin.debugDrawChunkCells(renderer,
                                  chunk_3,
                                  camX_px, camY_px,
                                  screen_w, screen_h,
                                  {255, 255, 0, 160},
                                  false);
        admin.debugDrawChunkCells(renderer,
                                  chunk_4,
                                  camX_px, camY_px,
                                  screen_w, screen_h,
                                  {255, 255, 0, 160},
                                  false);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // cap ~60fps
    }

    // --- Cleanup ---
    b2DestroyWorld(worldId);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
