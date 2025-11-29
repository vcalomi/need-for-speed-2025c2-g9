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
#include "lobby_auth_serializer.h"
#include "lobby_room_serializer.h"
#include "lobby_room_state_serializer.h"
#include "lobby_choose_car_serializer.h"
#include "lobby_maps_serializer.h"

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
                std::make_unique<PlayerLeftSerializer>();
        serializers[static_cast<uint8_t>(ActionCode::SEND_USERNAME)] =
                std::make_unique<AuthSerializer>(ActionCode::SEND_USERNAME);
        serializers[static_cast<uint8_t>(ActionCode::USERNAME_OK)] =
                std::make_unique<AuthSerializer>(ActionCode::USERNAME_OK);
        serializers[static_cast<uint8_t>(ActionCode::SEND_ERROR_MSG)] =
                std::make_unique<AuthSerializer>(ActionCode::SEND_ERROR_MSG);
        serializers[static_cast<uint8_t>(ActionCode::CREATE_ROOM)] =
                std::make_unique<RoomSerializer>(ActionCode::CREATE_ROOM);
        serializers[static_cast<uint8_t>(ActionCode::ROOM_CREATED)] =
                std::make_unique<RoomSerializer>(ActionCode::ROOM_CREATED);
        serializers[static_cast<uint8_t>(ActionCode::JOIN_ROOM)] =
                std::make_unique<RoomSerializer>(ActionCode::JOIN_ROOM);
        serializers[static_cast<uint8_t>(ActionCode::JOIN_OK)] =
                std::make_unique<RoomSerializer>(ActionCode::JOIN_OK);
        serializers[static_cast<uint8_t>(ActionCode::LIST_ROOMS)] =
                std::make_unique<RoomSerializer>(ActionCode::LIST_ROOMS);
        serializers[static_cast<uint8_t>(ActionCode::LIST_PLAYERS)] =
                std::make_unique<RoomStateSerializer>(ActionCode::LIST_PLAYERS);
        serializers[static_cast<uint8_t>(ActionCode::LIST_STATE)] =
                std::make_unique<RoomStateSerializer>(ActionCode::LIST_STATE);
        serializers[static_cast<uint8_t>(ActionCode::START_GAME)] =
                std::make_unique<RoomStateSerializer>(ActionCode::START_GAME);
        serializers[static_cast<uint8_t>(ActionCode::CHOOSE_CAR)] =
                std::make_unique<ChooseCarSerializer>(ActionCode::CHOOSE_CAR);
        serializers[static_cast<uint8_t>(ActionCode::CHOOSE_CAR_OK)] =
                std::make_unique<ChooseCarSerializer>(ActionCode::CHOOSE_CAR_OK);
        serializers[static_cast<uint8_t>(ActionCode::SELECT_MAPS)] =
                std::make_unique<MapsSerializer>(ActionCode::SELECT_MAPS);
        serializers[static_cast<uint8_t>(ActionCode::SELECT_MAPS_OK)] =
                std::make_unique<MapsSerializer>(ActionCode::SELECT_MAPS_OK);

        return serializers;
    }
};

#endif