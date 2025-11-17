#ifndef SERIALIZER_MAP_H
#define SERIALIZER_MAP_H

#include <cstdint>
#include <map>
#include <memory>

#include "../common_codes.h"

#include "checkpoint_serializer.h"
#include "initial_race_map_serializer.h"
#include "lap_completed_serializer.h"
#include "player_move_serializer.h"
#include "player_race_finished_serializer.h"
#include "player_serializer.h"
#include "race_finished_serializer.h"
#include "serializer.h"
#include "vehicle_checkpoint_serializer.h"
#include "vehicle_collision_serializer.h"
#include "vehicle_exploded_serializer.h"
#include "vehicle_serializer.h"
#include "vehicle_wall_collision_serializer.h"
#include "player_left_serializer.h"

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
        serializers[static_cast<uint8_t>(ActionCode::SEND_LAP_COMPLETED)] =
                std::make_unique<LapCompletedSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_RACE_FINISHED)] =
                std::make_unique<RaceFinishedSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_EXPLODED)] =
                std::make_unique<VehicleExplodedSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_INITIAL_RACE_MAP)] =
                std::make_unique<InitialRaceMapSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER_RACE_FINISHED)] =
                std::make_unique<PlayerRaceFinishedSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_PLAYER_LEFT)] =
                std::make_unique<PlayerRaceFinishedSerializer>();

        return serializers;
    }
};

#endif