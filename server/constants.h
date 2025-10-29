#pragma once

const float MIN_SPEED_STEER = 0.5f; 

enum TurnDir { 
    Left = -1, 
    None = 0, 
    Right = 1 
};

enum VehicleType {
    fiat_600,
    ferrari_F40,
    porsche_911,
    sedan,
    jeep,
    f100,
    truck
};