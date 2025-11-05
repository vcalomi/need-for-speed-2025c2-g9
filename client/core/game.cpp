#include "./game.h"

#include <sstream>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/vehicle.h"

Game::Game(Client& client):
        client_(client),
        engine_(),
        audioManager_(),
        resources_(engine_.GetRenderer()),
        world_(),
        inputSystem_(),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites()),
        map_(engine_.GetRenderer(), "../client/assets/need-for-speed/cities/san_andreas.png") {
    audioManager_.PlayBackgroundMusic("../client/assets/need-for-speed/music/background.wav");
}

void Game::processDto(const std::shared_ptr<Dto>& dto) {
    ActionCode code = static_cast<ActionCode>(dto->return_code());
    switch (code) {
        case ActionCode::SEND_PLAYER: {
            auto playerDto = std::dynamic_pointer_cast<PlayerDto>(dto);
            if (playerDto) {
                std::cout << "Procesando jugador id=" << (int)playerDto->id << std::endl;
                if (!world_.HasPlayer(playerDto->id)) {
                    world_.AddPlayer(playerDto->id, playerDto->Type, false);
                    std::cout << "Jugador agregado al mundo, id=" << (int)playerDto->id
                              << std::endl;
                }
            }
            break;
        }
        case ActionCode::SEND_CARS: {
            auto vehicleDto = std::dynamic_pointer_cast<VehicleDto>(dto);
            if (vehicleDto) {
                // std::cout << "Procesando vehículo id=" << (int)vehicleDto->id << " pos("
                //           << vehicleDto->x << ", " << vehicleDto->y
                //           << ") rot=" << vehicleDto->rotation << std::endl;
                world_.UpdateFromServer(vehicleDto->id, vehicleDto->x, vehicleDto->y,
                                        vehicleDto->rotation);
            }
            break;
        }
        default:
            std::cout << "DTO no manejado, code=" << (int)code << std::endl;
            break;
    }
}


void Game::Run() {
    bool running = true;
    while (running) {

        // --- INPUT ---
        inputSystem_.PollEvents(running);
        Dto input = inputSystem_.GetInputByte(this->world_.GetLocalPlayer().GetId());
        if (input.return_code() != 0) {
            client_.getSenderQueue().try_push(std::make_shared<Dto>(input));
            std::cout << "Input enviado: " << +input.return_code() << std::endl;
        }

        // --- MENSAJES DEL SERVIDOR ---
        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            processDto(dto);
        }

        // --- RENDERIZADO ---
        rendererSystem_.Render(world_, map_);

        SDL_Delay(16);
    }
    client_.stop();
}
