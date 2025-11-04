<<<<<<< HEAD:server/physics/vehicle.h
#pragma once
#include <box2d/box2d.h>
#include "../VehicleSpec.h"
#include "spawn.h"
#include "../constants.h"
#include <SDL.h>

class Vehicle {
public:
    Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id);

    void accelerate();
    void brake();
    void turn(TurnDir direction);
    void getPosition(float& x, float& y, float& angle) const;
    b2BodyId get_body();
    float width()  const noexcept { return spec_.width_m;  }
    float height() const noexcept { return spec_.height_m; }
    void draw(SDL_Renderer* r, float camX_px, float camY_px, float zoom, float ppm) const;
private:
    b2WorldId   world_id_;
    VehicleSpec spec_;      
    Spawn       spawn_;     
    int         vehicle_id_;
    b2BodyId    body_;
};

=======
#pragma once
#include <SDL.h>
#include <box2d/box2d.h>

#include "VehicleSpec.h"
#include "constants.h"
#include "spawn.h"

class Vehicle {
public:
    Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id);

    void accelerate();
    void brake();
    void turn(TurnDir direction);
    void getPosition(float& x, float& y, float& angle) const;
    b2BodyId get_body();
    float width() const noexcept { return spec_.width_m; }
    float height() const noexcept { return spec_.height_m; }
    void draw(SDL_Renderer* r, float camX_px, float camY_px, float zoom, float ppm) const;

private:
    b2WorldId world_id_;
    VehicleSpec spec_;
    Spawn spawn_;
    int vehicle_id_;
    b2BodyId body_;
};
>>>>>>> e908e5a4e799a79beb51cd882cd24021cbabe1e7:server/vehicle.h
