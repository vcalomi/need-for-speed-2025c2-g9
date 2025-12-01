#pragma once
#include <memory>
#include <string>

#include "../events/event.h"
#include "../network/client.h"
#include "../resources/resource_loader.h"
#include "../systems/audio_system.h"
#include "../systems/dto_handler_system.h"
#include "../systems/input_system.h"
#include "../systems/network_system.h"
#include "../systems/renderer_system.h"
#include "../world/map.h"
#include "../world/progress_manager.h"
#include "../world/world.h"
#include "./engine.h"

class Game {
public:
    explicit Game(Client& client);
    void Run();

private:
    Client& client_;
    Engine engine_;
    ResourceLoader resources_;
    EventBus eventBus_;
    AudioSystem audioSystem_;
    World world_;
    ProgressManager progress_;
    InputSystem inputSystem_;
    RendererSystem rendererSystem_;
    NetworkSystem networkSystem_;
    Map map_;
    DtoHandlerSystem dtoHandlerSystem_;
};
