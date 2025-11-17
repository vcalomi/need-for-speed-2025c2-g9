#include "./dto_handler_system.h"

#include "../../common/Dto/checkpoint.h"
#include "../../common/Dto/initial_race_map.h"
#include "../../common/Dto/lap_completed.h"
#include "../../common/Dto/player_race_finished.h"
#include "../../common/Dto/race_finished.h"
#include "../../common/Dto/vehicle_checkpoint.h"
#include "../../common/Dto/vehicle_collision.h"
#include "../../common/Dto/vehicle_exploded.h"
#include "../../common/Dto/vehicle_wall_collision.h"
#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/lap_completed_event.h"
#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"
#include "../events/player_race_finished_event.h"
#include "../events/race_finished_event.h"
#include "../events/race_info_event.h"
#include "../events/vehicle_exploded_event.h"
#include "../events/wall_collision_event.h"
#include "dto_handlers/send_cars_handler.h"
#include "dto_handlers/send_checkpoints_handler.h"
#include "dto_handlers/send_initial_race_map_handler.h"
#include "dto_handlers/send_lap_completed_handler.h"
#include "dto_handlers/send_player_handler.h"
#include "dto_handlers/send_player_race_finished_handler.h"
#include "dto_handlers/send_race_finished_handler.h"
#include "dto_handlers/send_vehicle_checkpoint_handler.h"
#include "dto_handlers/send_vehicle_exploded_handler.h"
#include "dto_handlers/send_vehicle_wall_collision_handler.h"
#include "dto_handlers/send_vehicles_collision_handler.h"

DtoHandlerSystem::DtoHandlerSystem(Client& client, EventBus& eventBus):
        client_(client), eventBus_(eventBus) {
    RegisterHandlers();
}


void DtoHandlerSystem::RegisterHandlers() {
    handlers_[static_cast<uint8_t>(ActionCode::SEND_PLAYER)] =
            std::make_unique<SendPlayerHandler>(client_);

    handlers_[static_cast<uint8_t>(ActionCode::SEND_CARS)] = std::make_unique<SendCarsHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_CHECKPOINTS)] =
            std::make_unique<SendCheckpointsHandler>();
    handlers_[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_CHECKPOINT)] =
            std::make_unique<SendVehicleCheckpointHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_WALL_COLLISION)] =
            std::make_unique<SendVehicleWallCollisionHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_VEHICLES_COLLISION)] =
            std::make_unique<SendVehiclesCollisionHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_LAP_COMPLETED)] =
            std::make_unique<SendLapCompletedHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_PLAYER_RACE_FINISHED)] =
            std::make_unique<SendPlayerRaceFinishedHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_RACE_FINISHED)] =
            std::make_unique<SendRaceFinishedHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_VEHICLE_EXPLODED)] =
            std::make_unique<SendVehicleExplodedHandler>();

    handlers_[static_cast<uint8_t>(ActionCode::SEND_INITIAL_RACE_MAP)] =
            std::make_unique<SendInitialRaceMapHandler>();
}


void DtoHandlerSystem::Process(const std::shared_ptr<Dto>& dto) {
    if (!dto)
        return;

    auto it = handlers_.find(dto->return_code());

    if (it == handlers_.end()) {
        std::cout << "[DtoHandler] Unhandled DTO code=" << (int)dto->return_code() << std::endl;
        return;
    }

    auto event = it->second->Handle(dto);
    if (event) {
        eventBus_.Publish(*event);
    }
}
