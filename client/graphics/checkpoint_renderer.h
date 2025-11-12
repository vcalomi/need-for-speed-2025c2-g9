#pragma once
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../world/camera.h"
#include "../world/checkpoint.h"

class CheckpointRenderer {
public:
    explicit CheckpointRenderer(SDL2pp::Renderer& renderer);
    void Draw(const std::vector<Checkpoint>& checkpoints, const Camera& camera);

private:
    SDL2pp::Renderer& renderer_;
    void DrawCircle(int x, int y, int radius);
    void DrawAnimated(int x, int y, int radius);
};
