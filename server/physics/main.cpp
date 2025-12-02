#include <iostream>

#include <SDL2pp/SDL2pp.hh>
#include <box2d/box2d.h>

#include "../constants.h"

#include "LevelCreator.h"

constexpr int WIN_W = 800;
constexpr int WIN_H = 600;

int main() {
    using namespace SDL2pp;

    // --- Init SDL ---
    SDL sdl(SDL_INIT_VIDEO);

    Window window("Debug Colisiones", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H,
                  SDL_WINDOW_SHOWN);
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- Cargar imagen de fondo ---
    Texture bgTex(renderer, PHYSICS_LEVELS_DIR "/../city.png");
    int bgW = bgTex.GetWidth();
    int bgH = bgTex.GetHeight();

    // --- Mundo físico (solo porque LevelCreator lo necesita) ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId world = b2CreateWorld(&wdef);

    // --- Cargar colisiones desde los .ldtkl ---
    LevelCreator lc;

    // Ojo con el path / mayúsculas (ajustá según tu carpeta real)
    lc.processDirectoryLevel(PHYSICS_LEVELS_DIR + std::string("/San_Andreas"));
    lc.createLevelCollision(world, lc.levels());

    // --- Cámara / zoom ---
    float camX_px = 0.0f;
    float camY_px = 0.0f;
    float zoom = 1.0f;

    bool running = true;
    SDL_Event e;

    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 last = SDL_GetPerformanceCounter();

    while (running) {
        // --- Input ---
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float elapsed = float(double(now - last) / double(freq));
        last = now;

        const Uint8* ks = SDL_GetKeyboardState(nullptr);

        // Mover cámara con WASD
        float camSpeed = 400.0f;  // px/seg
        if (ks[SDL_SCANCODE_W])
            camY_px -= camSpeed * elapsed;
        if (ks[SDL_SCANCODE_S])
            camY_px += camSpeed * elapsed;
        if (ks[SDL_SCANCODE_A])
            camX_px -= camSpeed * elapsed;
        if (ks[SDL_SCANCODE_D])
            camX_px += camSpeed * elapsed;

        // Zoom simple con Q/E (opcional)
        if (ks[SDL_SCANCODE_Q])
            zoom *= 1.0f + 0.5f * elapsed;  // zoom in
        if (ks[SDL_SCANCODE_E])
            zoom *= 1.0f - 0.5f * elapsed;  // zoom out

        if (zoom < 0.1f)
            zoom = 0.1f;

        // --- Render ---
        renderer.SetDrawColor(20, 20, 20, 255);
        renderer.Clear();

        // Dibujar fondo con cámara y zoom
        SDL_FRect dstBg{-camX_px * zoom, -camY_px * zoom, bgW * zoom, bgH * zoom};
        SDL_RenderCopyF(renderer.Get(), bgTex.Get(), nullptr, &dstBg);

        // Dibujar tiles de colisión encima
        lc.drawDebugTiles(renderer.Get(), camX_px, camY_px, zoom);

        renderer.Present();
        SDL_Delay(1);
    }

    b2DestroyWorld(world);
    return 0;
}
