#include "./dto_handler_system.h"

#include "../../common/Dto/checkpoint.h"
#include "../events/checkpoint_event.h"
#include "../events/player_events.h"

DtoHandlerSystem::DtoHandlerSystem(World& world, Client& client, EventBus& eventBus):
        world_(world), client_(client), eventBus_(eventBus) {}

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

            event = std::make_shared<PlayerStateUpdatedEvent>(
                    vehicleDto->username, vehicleDto->x, vehicleDto->y, vehicleDto->rotation,
                    vehicleDto->isAccelerating, vehicleDto->isBraking);


            break;
        }
        case ActionCode::SEND_CHECKPOINTS: {
            auto checkpointDto = std::dynamic_pointer_cast<CheckpointDto>(dto);
            if (!checkpointDto)
                return;
            std::cout << "[DtoHandler] New checkpoint: " << checkpointDto->id << " pos("
                      << checkpointDto->x << ", " << checkpointDto->y << ")" << std::endl;

            event = std::make_shared<CheckPointEvent>(checkpointDto->id, checkpointDto->x,
                                                      checkpointDto->y);
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
