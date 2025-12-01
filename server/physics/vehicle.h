#pragma once
#include <SDL2pp/SDL2pp.hh>
#include <box2d/box2d.h>

#include "../VehicleSpec.h"
#include "../constants.h"

#include "EntityTags.h"
#include "spawn.h"

class Vehicle {
public:
    Vehicle(b2WorldId worldId, VehicleSpec spec, Spawn spawn, int player_id, FixtureTag* tag);

    void accelerate();
    void brake();
    void turn(TurnDir direction);
    void getPosition(float& x, float& y, float& angle) const;
    b2BodyId get_body();
    float width() const noexcept { return spec_.width_m; }
    float height() const noexcept { return spec_.height_m; }
    void draw(SDL_Renderer* r, float camX_px, float camY_px, float zoom, float ppm) const;
    void setFixtureTag(FixtureTag* tag);
    void disableControl() { controlsEnabled_ = false; }

    float getVehicleHp() { return vehicle_hp_; }
    void applyDamage(float damage);

    void enableControl() { controlsEnabled_ = true; }

    bool getUnderBridge() { return under_bridge; }

    void setUnderBridge(bool setter) { under_bridge = setter; }
    int getVehicleId() { return vehicle_id_; }

    void setInfiniteHp() { infinite_hp_ = true; }

private:
    bool controlsEnabled_ = true;
    b2WorldId world_id_;
    VehicleSpec spec_;
    Spawn spawn_;
    int vehicle_id_;
    b2BodyId body_;
    float vehicle_hp_;
    bool under_bridge;
    bool infinite_hp_ = false;
};
