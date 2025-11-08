#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <box2d/box2d.h>

#include "../YamlParser.h"
#include "../constants.h"

#include "LevelCreator.h"
#include "vehicle.h"

constexpr int WIN_W = 1500;
constexpr int WIN_H = 1500;
constexpr float CAM_SPEED_PX = 600.0f;  // velocidad de cámara en píxeles/seg

int main() {
    // --- init specs ----
    YamlParser parser;
    auto mapa_config = parser.parse("../server/vehicles_specs/vehicle_specs.yaml");
    VehicleSpec ferrari_spec = mapa_config["ferrari_F40"];

    // --- Init SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL error: " << SDL_GetError() << "\n";
        return 1;
    }

    // --- Init SDL_image ---
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init error: " << IMG_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Debug Tiles",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_W,
        WIN_H,
        0
    );
    if (!window) {
        std::cerr << "CreateWindow error: " << SDL_GetError() << "\n";
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        std::cerr << "CreateRenderer error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // --- Cargar imagen de fondo ---
    SDL_Texture* bgTex = IMG_LoadTexture(renderer, "../server/physics/city.png");
    if (!bgTex) {
        std::cerr << "Error cargando fondo: " << IMG_GetError() << "\n";
        // si falla, seguimos sin fondo
    }
    int bgW = 0;
    int bgH = 0;
    if (bgTex) {
        SDL_QueryTexture(bgTex, nullptr, nullptr, &bgW, &bgH);
    }

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
    Spawn spawn_1 = vec_de_spawns[0];

    std::cout << "Spawn:"
              << " x=" << spawn_1.x << " y=" << spawn_1.y << " angle=" << spawn_1.angle << '\n';

    // --- Crear auto ----
    Vehicle vehicle(world, ferrari_spec, spawn_1, 0);

    // --- Cámara / zoom (zoom fijo = 1.0) ---
    float camX_px = 65.0f;
    float camY_px = 11.0f;
    const float zoom = 1.0f;

    bool running = true;
    SDL_Event e;

    // dt fijo para Box2D
    const float dt = 1.0f / 60.0f;
    (void)dt; // por si no lo usás aún

    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();

    float vehicle_x;
    float vehicle_y;
    float vehicle_angle;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float elapsed = float(double(now - last) / double(freq));
        last = now;

        const Uint8* ks = SDL_GetKeyboardState(nullptr);

        // --- Input de auto (WASD) ---
        if (ks[SDL_SCANCODE_W])
            vehicle.accelerate();
        if (ks[SDL_SCANCODE_S])
            vehicle.brake();
        if (ks[SDL_SCANCODE_A])
            vehicle.turn(TurnDir::Right);
        if (ks[SDL_SCANCODE_D])
            vehicle.turn(TurnDir::Left);

        // --- Física ---
        b2World_Step(world, 1.0f / 60.0f, /*subSteps*/ 4);

        // --- Cámara manual con I J K L ---
        if (ks[SDL_SCANCODE_I])
            camY_px -= CAM_SPEED_PX * elapsed;  // arriba
        if (ks[SDL_SCANCODE_K])
            camY_px += CAM_SPEED_PX * elapsed;  // abajo
        if (ks[SDL_SCANCODE_J])
            camX_px -= CAM_SPEED_PX * elapsed;  // izquierda
        if (ks[SDL_SCANCODE_L])
            camX_px += CAM_SPEED_PX * elapsed;  // derecha

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        // 1) Fondo (coordenadas de mundo: se mueve con la cámara)
        if (bgTex) {
            SDL_Rect dst;
            dst.x = -static_cast<int>(camX_px);
            dst.y = -static_cast<int>(camY_px);
            dst.w = bgW;
            dst.h = bgH;
            SDL_RenderCopy(renderer, bgTex, nullptr, &dst);
        }

        // 2) Tiles debug (hitboxes) encima del fondo
        lc.drawDebugTiles(renderer, camX_px, camY_px, zoom);

        // 3) Auto encima de todo
        vehicle.draw(renderer, camX_px, camY_px, zoom, PPM);

        // Debug posición
        vehicle.getPosition(vehicle_x, vehicle_y, vehicle_angle);
        std::cout << "Vehicle Pos:"
                  << " x=" << vehicle_x << " y=" << vehicle_y << " angle=" << vehicle_angle << '\n';

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    // --- Cleanup ---
    b2DestroyWorld(world);

    if (bgTex) SDL_DestroyTexture(bgTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}
