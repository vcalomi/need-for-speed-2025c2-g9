#pragma once
#include <box2d/box2d.h>
#include "constants.h
#include "VehicleSpec.h"
#include "spawn.h"



class Vehicle {

    public:
        explicit Vehicle(b2WorldId worldId, const VehicleSpec& spec, const Spawn& spawn, int player_id);
        void accelerate();
        void brake();
        void turn();
        void getPossition(float& x, float& y, float& angle);

    private:
        int vehicle_id;
        b2WorldId word_id;
        b2BodyId body;

};

