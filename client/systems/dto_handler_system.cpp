#include "./dto_handler_system.h"

#include "../events/checkpoints_event.h"
#include "../events/player_events.h"

DtoHandlerSystem::DtoHandlerSystem(World& world, Client& client, EventBus& eventBus):
        world_(world), client_(client), eventBus_(eventBus) {}

void DtoHandlerSystem::Process(const std::shared_ptr<Dto>& dto) {
    if (!dto)
        return;

    Event event;

    ActionCode code = static_cast<ActionCode>(dto->return_code());
    switch (code) {
        case ActionCode::SEND_PLAYER: {
            auto playerDto = std::dynamic_pointer_cast<PlayerDto>(dto);
            if (!playerDto)
                return;

            const auto& username = playerDto->username;
            std::cout << "[DtoHandler] Received player: " << username << std::endl;

            if (!world_.HasPlayer(username)) {
                world_.AddPlayer(username, playerDto->Type, username == client_.getUsername());
                std::cout << "[DtoHandler] Player added: " << username << std::endl;
            }
            break;
        }

        case ActionCode::SEND_CARS: {
            auto vehicleDto = std::dynamic_pointer_cast<VehicleDto>(dto);
            if (!vehicleDto)
                return;

            std::cout << "[DtoHandler] Vehicle update: " << vehicleDto->username << " pos("
                      << vehicleDto->x << ", " << vehicleDto->y << ") rot=" << vehicleDto->rotation
                      << std::endl;

            world_.UpdateFromServer(vehicleDto->username, vehicleDto->x, vehicleDto->y,
                                    vehicleDto->rotation);

            event = PlayerStateUpdatedEvent(vehicleDto->username, vehicleDto->x, vehicleDto->y,
                                            vehicleDto->rotation, vehicleDto->isAccelerating,
                                            vehicleDto->isBraking);


            break;
        }
        case ActionCode::SEND_CHECKPOINTS: {
            auto checkpointsDto = std::dynamic_pointer_cast<CheckpointsDto>(dto);
            if (!checkpointsDto)
                return;

            event = CheckPointsEvent(checkpointsDto->id, checkpointsDto->x, checkpointsDto->y);
            std::cout << "[DtoHandler] Checkpoints received: " << checkpointsDto->checkpoints.size()
                      << " checkpoints." << std::endl;
            break;
        }

        default:
            std::cout << "[DtoHandler] Unhandled DTO code=" << (int)code << std::endl;
            break;
    }

    eventBus_.Publish(event);
}
