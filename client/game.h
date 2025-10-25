#pragma once
#include "./engine.h"
#include "./input_system.h"
#include "./map.h"
#include "./renderer_system.h"
#include "./resource_loader.h"
#include "./world.h"

class Game {
public:
    Game();
    void Run();

private:
    Engine engine_;
    ResourceLoader resources_;
    World world_;
    InputSystem inputSystem_;
    RendererSystem rendererSystem_;
    Map map_;
};
