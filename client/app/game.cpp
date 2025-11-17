#include "./game.h"

#include <sstream>
#include <string>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"
#include "../events/player_events.h"
#include "../ui/minimap.h"
#include "../world/camera.h"

Game::Game(Client& client):
        client_(client),
        engine_(),
        audioSystem_(),
        resources_(engine_.GetRenderer()),
        eventBus_(),
        inputSystem_(),
        world_(eventBus_),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites(), world_, eventBus_),
        networkSystem_(client_, eventBus_),
        map_(engine_.GetRenderer(), eventBus_),
        dtoHandlerSystem_(client_, eventBus_) {
    audioSystem_.PlayBackgroundMusic("../client/assets/need-for-speed/music/background.wav");
}


void Game::Run() {
    bool running = true;
    Camera camera(engine_.getWindowWidth(), engine_.getWindowHeight());
    Minimap minimap(engine_.GetRenderer(), map_, 100, 100);

    while (map_.IsLoaded() == false && world_.HasPlayers() == false) {
        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            dtoHandlerSystem_.Process(dto);
        }
    }

    while (running) {

        inputSystem_.PollEvents(running);
        PlayerMoveDto input =
                inputSystem_.GetInputByte(this->world_.GetLocalPlayer().GetUsername());
        eventBus_.Publish(PlayerMoveEvent(this->world_.GetLocalPlayer().GetUsername(),
                                          static_cast<ActionCode>(input.move)));

        std::shared_ptr<Dto> dto = nullptr;
        while (client_.getRecvQueue().try_pop(dto)) {
            dtoHandlerSystem_.Process(dto);
        }

        camera.Follow(world_.GetLocalPlayerX(), world_.GetLocalPlayerY(), map_.GetWidth(),
                      map_.GetHeight());

        rendererSystem_.Render(world_, map_, camera, minimap);
        SDL_Delay(16);
    }
    client_.stop();
}
