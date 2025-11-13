#include "./dto_handler_system.h"

#include "../../common/Dto/checkpoint.h"
#include "../../common/Dto/vehicle_checkpoint.h"
#include "../../common/Dto/vehicle_wall_collision.h"
#include "../events/checkpoint_completed_event.h"
#include "../events/checkpoint_event.h"
#include "../events/player_events.h"
#include "../events/player_joined_event.h"
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
                                                        username == client_.getUsername());
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

        default:
            std::cout << "[DtoHandler] Unhandled DTO code=" << (int)code << std::endl;
            break;
    }

    if (event) {
        eventBus_.Publish(*event);
    }
}
