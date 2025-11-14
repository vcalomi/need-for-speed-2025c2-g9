#include "./dto_handler_system.h"

#include "../../common/Dto/checkpoint.h"
#include "../../common/Dto/lap_completed.h"
#include "../../common/Dto/race_finished.h"
#include "../../common/Dto/vehicle_checkpoint.h"
#include "../../common/Dto/vehicle_collision.h"
#include "../../common/Dto/vehicle_wall_collision.h"
#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/lap_completed_event.h"
#include "../events/player_collision_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"
#include "../events/race_finished_event.h"
#include "../events/wall_collision_event.h"


DtoHandlerSystem::DtoHandlerSystem(Client& client, EventBus& eventBus):
        client_(client), eventBus_(eventBus) {}

void DtoHandlerSystem::Process(const std::shared_ptr<Dto>& dto) {
    if (!dto)
        return;

    std::shared_ptr<Event> event;


    ActionCode code = static_cast<ActionCode>(dto->return_code());
    switch (code) {
        case ActionCode::SEND_PLAYER: {
            auto playerDto = std::dynamic_pointer_cast<PlayerDto>(dto);
            if (!playerDto)
                return;

            const auto& username = playerDto->username;
            std::cout << "[DtoHandler] Received player: " << username << std::endl;

            event = std::make_shared<PlayerJoinedEvent>(username, playerDto->Type,
                                                        username == client_.getUsername(),
                                                        playerDto->car_hp);
            break;
        }

        case ActionCode::SEND_CARS: {
            auto vehicleDto = std::dynamic_pointer_cast<VehicleDto>(dto);
            if (!vehicleDto)
                return;

            std::cout << "[DtoHandler] Vehicle update: " << vehicleDto->username << " pos("
                      << vehicleDto->x << ", " << vehicleDto->y << ") rot=" << vehicleDto->rotation
                      << std::endl;

            event = std::make_shared<PlayerStateUpdatedEvent>(
                    vehicleDto->username, vehicleDto->x, vehicleDto->y, vehicleDto->rotation,
                    vehicleDto->isAccelerating, vehicleDto->isBraking);


            break;
        }
        case ActionCode::SEND_CHECKPOINTS: {
            auto checkpointDto = std::dynamic_pointer_cast<CheckpointDto>(dto);
            if (!checkpointDto)
                return;
            std::cout << "[DtoHandler] Checkpoint received: ID " << checkpointDto->id << " pos("
                      << checkpointDto->x << ", " << checkpointDto->y << ")" << std::endl;

            event = std::make_shared<CheckPointEvent>(checkpointDto->id, checkpointDto->x,
                                                      checkpointDto->y);
            break;
        }
        case ActionCode::SEND_VEHICLE_CHECKPOINT: {
            auto vehicleCheckpointDto = std::dynamic_pointer_cast<VehicleCheckpointDto>(dto);
            if (!vehicleCheckpointDto)
                return;

            std::cout << "[DtoHandler] Vehicle Checkpoint: " << vehicleCheckpointDto->username
                      << " checkpoint ID " << vehicleCheckpointDto->checkpointIndex << std::endl;

            event = std::make_shared<CheckPointCompletedEvent>(
                    vehicleCheckpointDto->username, vehicleCheckpointDto->checkpointIndex);
            break;
        }
        case ActionCode::SEND_VEHICLE_WALL_COLLISION: {
            auto wallCollisionDto = std::dynamic_pointer_cast<VehicleWallCollisionDto>(dto);
            if (!wallCollisionDto)
                return;

            std::cout << "[DtoHandler] Vehicle Wall Collision: " << wallCollisionDto->username
                      << std::endl;

            event = std::make_shared<WallCollisionEvent>(wallCollisionDto->username);
            break;
        }
        case ActionCode::SEND_VEHICLES_COLLISION: {
            auto vehicleCollisionDto = std::dynamic_pointer_cast<VehicleCollisionDto>(dto);
            if (!vehicleCollisionDto)
                return;

            std::cout << "[DtoHandler] Vehicle Collision: "
                      << vehicleCollisionDto->vehicle1_username << " with "
                      << vehicleCollisionDto->vehicle2_username << std::endl;

            event = std::make_shared<PlayerCollisionEvent>(vehicleCollisionDto->vehicle1_username,
                                                           vehicleCollisionDto->vehicle2_username);
            break;
        }
        case ActionCode::SEND_LAP_COMPLETED: {
            auto lapCompletedDto = std::dynamic_pointer_cast<LapCompletedDto>(dto);
            if (!lapCompletedDto)
                return;

            std::cout << "[DtoHandler] Lap Completed: " << lapCompletedDto->username
                      << " current lap " << lapCompletedDto->current_lap << std::endl;

            event = std::make_shared<LapCompletedEvent>(lapCompletedDto->username,
                                                        lapCompletedDto->current_lap);
            break;
        }
        case ActionCode::SEND_RACE_FINISHED: {
            auto raceFinishedDto = std::dynamic_pointer_cast<RaceFinishedDto>(dto);
            if (!raceFinishedDto)
                return;

            std::cout << "[DtoHandler] Race Finished: " << raceFinishedDto->username << std::endl;

            event = std::make_shared<RaceFinishedEvent>(raceFinishedDto->username);
            break;
        }

        default:
            std::cout << "[DtoHandler] Unhandled DTO code=" << (int)code << std::endl;
            break;
    }

    if (event) {
        eventBus_.Publish(*event);
    }
}
