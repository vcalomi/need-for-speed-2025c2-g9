#ifndef SERIALIZER_MAP_H
#define SERIALIZER_MAP_H

#include <map>
#include <memory>
#include <cstdint>

#include "../common_codes.h"
#include "serializer.h"
#include "checkpoint_serializer.h"
#include "player_move_serializer.h"
#include "vehicle_checkpoint_serializer.h"
#include "vehicle_serializer.h"
#include "vehicle_wall_collision_serializer.h"
#include "player_serializer.h"
#include "vehicle_collision_serializer.h"

class SerializerMap {
public:
    static std::map<uint8_t, std::unique_ptr<Serializer>> createSerializerMap() {
        std::map<uint8_t, std::unique_ptr<Serializer>> serializers;
        
        serializers[static_cast<uint8_t>(ActionCode::SEND_CARS)] =
                std::make_unique<VehicleSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER)] =
                std::make_unique<PlayerSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER_MOVE)] =
                std::make_unique<PlayerMoveSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_CHECKPOINTS)] =
                std::make_unique<CheckpointSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_CHECKPOINT)] =
                std::make_unique<VehicleCheckpointSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLES_COLLISION)] =
                std::make_unique<VehicleCollisionSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_WALL_COLLISION)] =
                std::make_unique<VehicleWallCollisionSerializer>();
        
        return serializers;
    }
};

#endif