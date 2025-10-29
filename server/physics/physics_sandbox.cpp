#include <box2d/box2d.h>
#include <SDL.h>
#include <iostream>
#include "../YamlParser.h"
#include "../vehicle.h"
#include "../spawn.h"
#include "../constants.h"
#include "LevelAdministrator.h"

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
    float cameraZoom = 6.0f;             // 1.0 = sin zoom, >1 acerca, <1 aleja
    float camX_px = 0.0f, camY_px = -200.0f; // offset de cámara en PIXELES de mundo (no metros)
    const float panSpeed_px = 400.0f;     // velocidad de paneo en px/seg (se escala con dt)
    
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;
    const double freq = (double)SDL_GetPerformanceFrequency();

    // --- Mundo Box2D (top-down) ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId worldId = b2CreateWorld(&wdef);

    
    // 🔹 Cargar y construir colisión del mapa
    LevelAdministrator lvl;
    std::string err;
    if (!lvl.loadCollisionGrid("../server/physics/Level_0.ldtkl", "", /*layerId=*/"Collision2px", &err)) {
        std::cerr << "[LDtk] " << err << "\n";
        // si querés, podés abortar aquí
    } else {
        LevelAdministrator::BuildConfig cfg;
        cfg.ppm = PPM;                 // 🔹 MUY IMPORTANTE: usar el mismo PPM
        cfg.mergeHorizontal = true;    // 🔹 menos fixtures
        cfg.flipY = true;           // ponelo true si invertís el eje Y en tu render
        cfg.levelPxHeight = 384;     // solo requerido si flipY=true
        b2BodyId staticCollision = lvl.buildCollision(worldId, cfg);
        (void)staticCollision; // solo para evitar warning si no lo usás
    }


    // --- Auto ---
    Vehicle vehicle(worldId, ferrari_spec, Spawn{-10.0f, -10.0f, 0.0f}, /*player_id=*/0);

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
            if (e.type == SDL_QUIT) running = false;

            // Zoom con rueda del mouse
            if (e.type == SDL_MOUSEWHEEL) {
                float factor = (e.wheel.y > 0) ? 1.1f : 1.0f/1.1f;
                cameraZoom *= factor;
                if (cameraZoom < 0.25f) cameraZoom = 0.25f;
                if (cameraZoom > 12.0f) cameraZoom = 12.0f;
            }
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

        // seguimiento suave
        float lerp = 5.0f * dt;  // cuanto más alto, más rápido sigue (5 a 10 va bien)
        camX_px += (world_x_px - camX_px) * lerp;
        camY_px += (world_y_px - camY_px) * lerp;
        

        // aplicar offset de cámara (en px de mundo) y zoom.
        // X pantalla: centro de la ventana + desplazamiento
        float x_px = (world_x_px - camX_px) * cameraZoom + screen_w * 0.5f;

        // Y pantalla: invertimos porque la pantalla crece hacia abajo
        float y_px =  screen_h * 0.5f - (world_y_px - camY_px) * cameraZoom;

        // rotación pantalla
        float a = -ang_rad; 
        float c = cosf(a), s = sinf(a);

        // Tamaños: (m -> px) y luego zoom
        float w_px = vehicle.width()  * PPM * cameraZoom;
        float h_px = vehicle.height() * PPM * cameraZoom;

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

        // 🔶 DEBUG: dibujar colisión del mapa
        lvl.debugDraw(renderer, camX_px, camY_px, cameraZoom, screen_w, screen_h,
        /*color*/ SDL_Color{255,128,0,80}); // naranja translúcido

        SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
        SDL_RenderPresent(renderer);

        // cap ~60fps
        SDL_Delay(16);
    }

    // --- Cleanup ---
    b2DestroyWorld(worldId);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}