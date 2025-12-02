#pragma once

const float MIN_SPEED_STEER = 0.05f;
const float PPM = 4.0f;
const float TILE_SIZE_PX = 4.0f;
const float CHECKPOINT_RADIUS_PX = 32.0f;
enum TurnDir { Left = -1, None = 0, Right = 1 };

enum VehicleType { fiat_600, ferrari_F40, porsche_911, sedan, jeep, f100, truck };
