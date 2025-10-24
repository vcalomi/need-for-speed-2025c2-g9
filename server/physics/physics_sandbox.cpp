#include <box2d/box2d.h>
#include <SDL.h>
#include <iostream>

const float MOVE_FORCE = 30.0f;       // Newtons 
const float STEER_TORQUE = 5.0f;      // N·m 
const float MIN_SPEED_STEER = 0.5f;   // m/s para permitir girar

int main() {

    // --- Init SDL ---
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Physics Sandbox",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1200, 800, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // --- Crear mundo (top-down) ---
    b2WorldDef wdef = b2DefaultWorldDef();
    wdef.gravity = {0.0f, 0.0f};
    b2WorldId worldId = b2CreateWorld(&wdef);

    // --- Crear un cuerpo dinámico ---
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {0.0f, 0.0f};
    bodyDef.linearDamping = 2.0f;
    bodyDef.angularDamping = 3.f;
    
    b2BodyId body = b2CreateBody(worldId, &bodyDef);

    // --- Crear un box shape (1x1 metro) ---
    b2Polygon boxShape = b2MakeBox(1.0f, 0.5f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    b2CreatePolygonShape(body, &shapeDef, &boxShape);

    bool running = true;
    SDL_Event e;
    const float pixels_per_meter = 50.0f;
    const float move_force = 10.0f;

    while (running) {
        while (SDL_PollEvent(&e)) if (e.type == SDL_QUIT) running = false;

        const Uint8* ks = SDL_GetKeyboardState(NULL);

         b2Vec2 forward = b2Body_GetWorldVector(body, (b2Vec2){0.0f, 1.0f});

        float thrust = 0.0f;
        if (ks[SDL_SCANCODE_UP])   thrust += MOVE_FORCE;
        if (ks[SDL_SCANCODE_DOWN]) thrust -= MOVE_FORCE;

        if (thrust != 0.0f) {
            b2Vec2 F = { forward.x * thrust, forward.y * thrust };
            b2Body_ApplyForceToCenter(body, F, true);
        }

        // Permitir girar solo si hay velocidad hacia adelante/atrás
        b2Vec2 v = b2Body_GetLinearVelocity(body);
        float speedForward = v.x * forward.x + v.y * forward.y; // dot(v, forward)

        if (fabsf(speedForward) > MIN_SPEED_STEER) {
            if (ks[SDL_SCANCODE_LEFT])  b2Body_ApplyTorque(body, +STEER_TORQUE, true);
            if (ks[SDL_SCANCODE_RIGHT]) b2Body_ApplyTorque(body, -STEER_TORQUE, true);
        }
        
        // --- Step del mundo ---
        b2World_Step(worldId, 1.0f / 60.0f, 8);

        // --- Mundo → pantalla ---
        b2Vec2 p = b2Body_GetPosition(body);
        float x = p.x * pixels_per_meter + 400;
        float y = 300 - p.y * pixels_per_meter;

        // --- Ángulo
        b2Rot rot = b2Body_GetRotation(body);
        float ang_rad_world = b2Rot_GetAngle(rot);

        // rotación en pantalla y invertido
        float a = -ang_rad_world;

        float c = cosf(a), s = sinf(a);

        const float hw = 25.0f;
        const float hh = 50.0f;

        SDL_FPoint local[4] = {
            {-hw, -hh}, { hw, -hh}, { hw,  hh}, {-hw,  hh}
        };

        // Rotar + trasladar
        SDL_Vertex verts[4];
        for (int i = 0; i < 4; ++i) {
            float rx =  local[i].x * c - local[i].y * s;
            float ry =  local[i].x * s + local[i].y * c;
            verts[i].position.x = x + rx;
            verts[i].position.y = y + ry;
            verts[i].color = {0, 255, 0, 255};
            verts[i].tex_coord = {0, 0};
        }

        // Triangulación (dos triángulos)
        int indices[6] = {0, 1, 2,  0, 2, 3};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderGeometry(renderer, nullptr, verts, 4, indices, 6);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // --- Cleanup ---
    b2DestroyWorld(worldId);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
