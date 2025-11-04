#pragma once
#include <string>

#include "../graphics/renderer_system.h"
#include "../input/input_system.h"
#include "../network/mock_server.h"
#include "../world/world.h"
#include "./audio_manager.h"
#include "./engine.h"
#include "./map.h"
#include "./resource_loader.h"

class Game {
public:
    Game();
    void Run();

private:
    Engine engine_;
    AudioManager audioManager_;
    ResourceLoader resources_;
    World world_;
    InputSystem inputSystem_;
    RendererSystem rendererSystem_;
    Map map_;
    MockServer mockServer_;

    void ProcessServerMessage(const std::string& msg);
};
