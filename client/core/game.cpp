#include "./game.h"

#include <sstream>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"

#include "camera.h"

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
                    world_.AddPlayer(playerDto->id, playerDto->Type, true);
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
    Camera camera;

    std::shared_ptr<Dto> player = nullptr;
    client_.getRecvQueue().try_pop(player);
    processDto(player);

    while (running) {

        camera.x = world_.GetLocalPlayer().GetX() - (800 / 2);
        camera.y = world_.GetLocalPlayer().GetY() - (600 / 2);

        // --- INPUT ---
        inputSystem_.PollEvents(running);
        PlayerMoveDto input = inputSystem_.GetInputByte(this->world_.GetLocalPlayer().GetId());
        if (static_cast<ActionCode>(input.move) != ActionCode::IDLE) {
            client_.getSenderQueue().try_push(std::make_shared<PlayerMoveDto>(input));
            std::cout << "Input enviado: " << static_cast<int>(input.move) << std::endl;
        }

        // --- MENSAJES DEL SERVIDOR ---
        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            processDto(dto);
        }

        // --- RENDERIZADO ---
        rendererSystem_.Render(world_, map_, camera);
        SDL_Delay(16);
    }
    client_.stop();
}
