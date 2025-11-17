#include "background_renderer.h"

BackgroundRenderer::BackgroundRenderer(SDL2pp::Renderer& renderer): renderer_(renderer) {}

void BackgroundRenderer::RenderBackground(Map& map, const Camera& camera) {
    map.RenderBackground(camera);
}

void BackgroundRenderer::RenderForeground(Map& map, const Camera& camera) {
    map.RenderForeground(camera);
}
