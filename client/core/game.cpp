#include "./game.h"

#include <sstream>
#include <string>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"
#include "../ui/minimap.h"

#include "camera.h"

Game::Game(Client& client):
        client_(client),
        engine_(),
        audioManager_(),
        resources_(engine_.GetRenderer()),
        world_(),
        inputSystem_(),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites()),
        map_(engine_.GetRenderer(), "../client/assets/need-for-speed/cities/liberty_city.png") {
    audioManager_.PlayBackgroundMusic("../client/assets/need-for-speed/music/background.wav");
}

void Game::processDto(const std::shared_ptr<Dto>& dto) {
    ActionCode code = static_cast<ActionCode>(dto->return_code());
    switch (code) {
        case ActionCode::SEND_PLAYER: {
            auto playerDto = std::dynamic_pointer_cast<PlayerDto>(dto);
            if (playerDto) {
                std::cout << "Procesando jugador username=" << playerDto->username << std::endl;
                auto playerUsername = playerDto->username;
                if (!world_.HasPlayer(playerUsername)) {
                    world_.AddPlayer(playerUsername, playerDto->Type,
                                     playerUsername == client_.getUsername());
                    std::cout << "Jugador agregado al mundo, username=" << playerUsername
                              << std::endl;
                }
            }
            break;
        }
        case ActionCode::SEND_CARS: {
            auto vehicleDto = std::dynamic_pointer_cast<VehicleDto>(dto);
            if (vehicleDto) {
                std::cout << "Procesando vehículo username=" << vehicleDto->username << " pos("
                          << vehicleDto->x << ", " << vehicleDto->y
                          << ") rot=" << vehicleDto->rotation << std::endl;
                world_.UpdateFromServer(vehicleDto->username, vehicleDto->x, vehicleDto->y,
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
    Camera camera(engine_.getWindowWidth(), engine_.getWindowHeight());
    Minimap minimap(engine_.GetRenderer(), map_, 100, 100);

    while (world_.HasPlayers() == false) {
        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            processDto(dto);
        }
    }

    while (running) {

        // --- INPUT ---
        inputSystem_.PollEvents(running);
        PlayerMoveDto input =
                inputSystem_.GetInputByte(this->world_.GetLocalPlayer().GetUsername());
        if (static_cast<ActionCode>(input.move) != ActionCode::IDLE) {
            client_.getSenderQueue().try_push(std::make_shared<PlayerMoveDto>(input));
        }

        // --- MENSAJES DEL SERVIDOR ---
        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            processDto(dto);
        }

        // CAMERA UPDATE
        camera.Follow(world_.GetLocalPlayerX(), world_.GetLocalPlayerY(), map_.GetWidth(),
                      map_.GetHeight());

        // --- RENDERIZADO ---
        rendererSystem_.Render(world_, map_, camera, minimap);
        SDL_Delay(16);
    }
    client_.stop();
}
