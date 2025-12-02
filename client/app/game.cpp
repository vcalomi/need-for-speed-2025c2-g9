#include "./game.h"

#include <sstream>
#include <string>

#include "../../common/Dto/dto.h"
#include "../../common/Dto/player.h"
#include "../../common/Dto/player_move.h"
#include "../../common/Dto/vehicle.h"
#include "../events/cheat_end_race_event.h"
#include "../events/close_game_event.h"
#include "../events/infinite_health_event.h"
#include "../events/player_events.h"
#include "../events/player_left_event.h"
#include "../ui/minimap.h"
#include "../world/camera.h"

Game::Game(Client& client):
        client_(client),
        engine_(),
        resources_(engine_.GetRenderer()),
        eventBus_(),
        audioSystem_(eventBus_),
        inputSystem_(),
        world_(eventBus_),
        progress_(eventBus_),
        rendererSystem_(engine_.GetRenderer(), resources_.GetCarSprites(),
                        resources_.GetNpcSprites(), world_, eventBus_, progress_),
        networkSystem_(client_, eventBus_),
        dtoHandlerSystem_(client_, eventBus_),
        map_(engine_.GetRenderer(), eventBus_) {
    audioSystem_.PlayBackgroundMusic("../client/assets/need-for-speed/music/background.wav");

    eventBus_.Subscribe<CloseGameEvent>([this](const CloseGameEvent& e) {
        std::cout << "[Game] CloseGameEvent received. Stopping the game loop.\n";
        client_.stop();
    });
}


void Game::Run() {
    bool running = true;
    Camera camera(engine_.getWindowWidth(), engine_.getWindowHeight());


    while (map_.IsLoaded() == false && world_.HasPlayers() == false) {
        std::shared_ptr<Dto> dto = nullptr;
        try {
            while (client_.getRecvQueue().try_pop(dto)) {
                dtoHandlerSystem_.Process(dto);
            }
        } catch (const ClosedQueue&) {
            break;
        }
    }
    Minimap minimap(engine_.GetRenderer(), 100, 100);
    progress_.SetCheckpoints(world_.GetCheckpoints());
    progress_.SetLocalPlayer(world_.GetLocalPlayer().GetUsername());

    while (running) {

        inputSystem_.PollEvents(running);

        if (inputSystem_.IsKeyPressed(SDL_SCANCODE_Q)) {
            eventBus_.Publish(InfiniteHealthEvent(this->world_.GetLocalPlayer().GetUsername()));
        }

        if (inputSystem_.WasKeyPressed(SDL_SCANCODE_E)) {
            eventBus_.Publish(CheatEndRaceEvent(world_.GetLocalPlayer().GetUsername()));
        }


        PlayerMoveDto input =
                inputSystem_.GetInputByte(this->world_.GetLocalPlayer().GetUsername());
        eventBus_.Publish(PlayerMoveEvent(this->world_.GetLocalPlayer().GetUsername(),
                                          static_cast<ActionCode>(input.move)));

        std::shared_ptr<Dto> dto = nullptr;
        try {
            while (client_.getRecvQueue().try_pop(dto)) {
                dtoHandlerSystem_.Process(dto);
            }
        } catch (const ClosedQueue&) {
            break;
        }

        int realW, realH;
        SDL_GetRendererOutputSize(engine_.GetRenderer().Get(), &realW, &realH);
        camera.SetViewport(realW, realH);
        camera.Follow(world_.GetLocalPlayerX(), world_.GetLocalPlayerY(), map_.GetWidth(),
                      map_.GetHeight());

        rendererSystem_.Render(world_, map_, camera, minimap);
        SDL_Delay(16);
    }

    eventBus_.Publish(PlayerLeftEvent(this->world_.GetLocalPlayer().GetUsername()));
    client_.stop();
}
