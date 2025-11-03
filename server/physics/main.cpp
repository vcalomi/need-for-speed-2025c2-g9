#include <SDL.h>
#include <box2d/box2d.h>
#include "LevelCreator.h"
#include <iostream>
#include "../YamlParser.h"
#include "../vehicle.h"
#include "../constants.h"   // TurnDir si lo declaraste ahí

constexpr float PPM = 50.0f;
constexpr int   WIN_W = 1500;
constexpr int   WIN_H = 1500;
constexpr float CAM_SPEED_PX = 600.0f;   // velocidad de cámara en píxeles/seg

int main() {
    // --- init specs ----
    YamlParser parser;
    auto mapa_config = parser.parse("../server/vehicle_specs.yaml");
    VehicleSpec ferrari_spec = mapa_config["ferrari_F40"];

    // --- Init SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Debug Tiles",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- Crear mundo vacío ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId world = b2CreateWorld(&wdef);

    // --- Cargar niveles y colisiones ---
    const char* kDir = "../server/physics/Levels";
    LevelCreator lc;
    lc.processDirectoryLevel(kDir);
    lc.createLevelCollision(world, lc.levels());
    
    std::vector<Spawn> vec_de_spawns = lc.getSpawnPoints();

    Spawn spawn_1 =  vec_de_spawns[0];

    std::cout << "Spawn:"
          << " x=" << spawn_1.x
          << " y=" << spawn_1.y
          << " angle=" << spawn_1.angle
          << '\n';

    // --- Crear auto ----
    Vehicle vehicle(world, ferrari_spec, spawn_1, 0);

    // --- Cámara / zoom (zoom fijo = 1.0) ---
    float camX_px = 65.0f;
    float camY_px = 11.0f;
    const float zoom = 8.0f;

    bool running = true;
    SDL_Event e;

    // dt fijo para Box2D
    const float dt = 1.0f / 60.0f;
    // timing simple para mover cámara con velocidad constante
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();
    

    float  vehicle_x;
    float vehicle_y;
    float vehicle_angle;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = float(double(now - last) / double(freq));
        last = now;

        // --- Input de auto (WASD) ---
        const Uint8* ks = SDL_GetKeyboardState(nullptr);
        if (ks[SDL_SCANCODE_W]) vehicle.accelerate();
        if (ks[SDL_SCANCODE_S]) vehicle.brake();
        if (ks[SDL_SCANCODE_A]) vehicle.turn(TurnDir::Right);
        if (ks[SDL_SCANCODE_D]) vehicle.turn(TurnDir::Left);

        // --- Física ---
        b2World_Step(world, 1.0f/60.0f, /*subSteps*/ 4);

        // --- Cámara manual con I J K L ---
        if (ks[SDL_SCANCODE_I]) camY_px -= CAM_SPEED_PX * dt; // arriba
        if (ks[SDL_SCANCODE_K]) camY_px += CAM_SPEED_PX * dt; // abajo
        if (ks[SDL_SCANCODE_J]) camX_px -= CAM_SPEED_PX * dt; // izquierda
        if (ks[SDL_SCANCODE_L]) camX_px += CAM_SPEED_PX * dt; // derecha

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        lc.drawDebugTiles(renderer, camX_px, camY_px, zoom);
        vehicle.draw(renderer, camX_px, camY_px, zoom, PPM);

        
        vehicle.getPosition(vehicle_x, vehicle_y, vehicle_angle);
        
            std::cout << "Vehicle Poss:"
          << " x=" << vehicle_x
          << " y=" << vehicle_y
          << " angle=" << vehicle_angle
          << '\n';

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    b2DestroyWorld(world);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}