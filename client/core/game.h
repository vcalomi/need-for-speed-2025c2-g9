#pragma once
#include <memory>
#include <string>

#include "../events/event.h"
#include "../graphics/renderer_system.h"
#include "../input/input_system.h"
#include "../network/client.h"
#include "../world/world.h"
#include "./audio_manager.h"
#include "./dto_handler_system.h"
#include "./engine.h"
#include "./map.h"
#include "./network_system.h"
#include "./resource_loader.h"

class Game {
public:
    explicit Game(Client& client);
    void Run();

private:
    Client& client_;
    Engine engine_;
    AudioManager audioManager_;
    ResourceLoader resources_;
    EventBus eventBus_;
    World world_;
    InputSystem inputSystem_;
    RendererSystem rendererSystem_;
    NetworkSystem networkSystem_;
    Map map_;
    DtoHandlerSystem dtoHandlerSystem_;

    void processDto(const std::shared_ptr<Dto>& dto);
};
