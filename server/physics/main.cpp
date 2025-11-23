#include <iostream>

#include <SDL2pp/SDL2pp.hh>
#include <box2d/box2d.h>

#include "../YamlParser.h"
#include "../constants.h"

#include "LevelCreator.h"
#include "vehicle.h"

#include "PhysicsEventCollector.h"
#include "EntityTags.h"

constexpr int WIN_W = 800;
constexpr int WIN_H = 600;

int main() {
    using namespace SDL2pp;

    // --- Init SDL y SDL_image ---
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Debug Tiles (SDL2pp)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W,
                  WIN_H, SDL_WINDOW_SHOWN);
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- Cargar imagen de fondo ---
    Texture bgTex(renderer, PHYSICS_LEVELS_DIR "/../city.png");
    int bgW = bgTex.GetWidth();
    int bgH = bgTex.GetHeight();
    std::cout << "Fondo cargado: " << bgW << "x" << bgH << std::endl;

    // --- Mundo físico ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId world = b2CreateWorld(&wdef);

    // --- Colisiones ---
    LevelCreator lc;
    lc.processDirectoryLevel(PHYSICS_LEVELS_DIR "/Liberty_City");
    lc.createLevelCollision(world, lc.levels());

    PhysicsEventCollector collector;

    std::vector<Spawn> spawns = lc.getSpawnPoints();
    
    if (spawns.empty()) {
        std::cerr << "Error: No hay spawns disponibles. Saliendo..." << std::endl;
        return 1;
    }
    
    Spawn spawn = spawns[0];

    YamlParser parser;
    auto mapa_config = parser.parse(VEHICLES_SPECS_DIR "/vehicle_specs.yaml");
    VehicleSpec ferrari_spec = mapa_config["ferrari_F40"];

    std::deque<FixtureTag> vehicle_tags;
    auto* vtag = makeTag(vehicle_tags, EntityKind::Vehicle, /*playerId=*/0);

    Vehicle vehicle(world, ferrari_spec, spawn, 0, vtag);
    

    // --- Cámara / zoom ---
    float camX_px = 0.0f;
    float camY_px = 0.0f;
    const float zoom = 1.0f;

    bool running = true;
    SDL_Event e;
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();

    float vehicle_x, vehicle_y, vehicle_angle;
    std::vector<CheckpointInfo> checkpoints = lc.get_checkpoints();

    std::cout << "Total checkpoints: " << checkpoints.size() << '\n';

    // --- Loop principal ---
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float elapsed = float(double(now - last) / double(freq));
        last = now;

        const Uint8* ks = SDL_GetKeyboardState(nullptr);

        // Movimiento del vehículo
        if (ks[SDL_SCANCODE_W])
            vehicle.accelerate();
        if (ks[SDL_SCANCODE_S])
            vehicle.brake();
        if (ks[SDL_SCANCODE_A])
            vehicle.turn(TurnDir::Right);
        if (ks[SDL_SCANCODE_D])
            vehicle.turn(TurnDir::Left);

        // --- Física ---
        b2World_Step(world, 1.0f / 60.0f, 4);
        
        collector.collect(world);

        // Mostrar/usar los eventos y limpiar
        auto events = collector.drain();
        
        for (const auto& ev : events) {
            if (auto* cp = std::get_if<RawVehicleCheckpoint>(&ev)) {
                std::cout << "[EV] Vehicle " << cp->vehicleId
                        << " hit checkpoint " << cp->checkpointIndex << "\n";
            } else if (auto* vv = std::get_if<RawVehicleVehicle>(&ev)) {
                std::cout << "[EV] Vehicle " << vv->a << " hit vehicle " << vv->b << "\n";
            } else if (auto* vw = std::get_if<RawVehicleWall>(&ev)) {
                std::cout << "[EV] Vehicle " << vw->vehicleId << " hit wall\n";
            }
        }

        // Obtener posición actual del vehículo
        vehicle.getPosition(vehicle_x, vehicle_y, vehicle_angle);

        //std::cout << "Vehículo pos(" << vehicle_x << ", " << vehicle_y
                  //<< ") ang=" << vehicle_angle * 180.0f / M_PI << std::endl;
        // --- Cámara sigue al vehículo ---
        camX_px = vehicle_x - WIN_W / 2.0f;
        camY_px = vehicle_y - WIN_H / 2.0f;

        // Limitar cámara a los bordes del mapa
        camX_px = std::max(0.0f, std::min(camX_px, static_cast<float>(bgW - WIN_W)));
        camY_px = std::max(0.0f, std::min(camY_px, static_cast<float>(bgH - WIN_H)));

        // --- Render ---
        renderer.SetDrawColor(20, 20, 20, 255);
        renderer.Clear();

        // Fondo (desplazado por cámara)
        Rect dst(-camX_px, -camY_px, bgW, bgH);
        renderer.Copy(bgTex, NullOpt, dst);

        // Colisiones
        lc.drawDebugTiles(renderer.Get(), camX_px, camY_px, zoom);
        lc.drawDebugCheckpoints(renderer.Get(), camX_px, camY_px, zoom);
        // Auto
        vehicle.draw(renderer.Get(), camX_px, camY_px, zoom, PPM);

        renderer.Present();
        SDL_Delay(1);
    }

    b2DestroyWorld(world);
}
