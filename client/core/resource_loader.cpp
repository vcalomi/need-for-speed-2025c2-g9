#include "./resource_loader.h"

ResourceLoader::ResourceLoader(SDL2pp::Renderer& renderer):
        carSprites_(renderer, "../client/assets/need-for-speed/cars/cars.png") {
    LoadCarSprites();
}

void ResourceLoader::LoadCarSprites() {
    carSprites_.AddSprite("JeepSur", {167, 304, 26, 39});
    carSprites_.AddSprite("JeepNorte", {167, 345, 26, 39});
    carSprites_.AddSprite("JeepEste", {0, 311, 39, 26});
    carSprites_.AddSprite("JeepOeste", {1, 351, 39, 26});
    carSprites_.AddSprite("JeepSureste20", {40, 307, 40, 36});
    carSprites_.AddSprite("JeepSuroeste20", {280, 307, 40, 36});
    carSprites_.AddSprite("JeepNoroeste20", {40, 345, 40, 36});
    carSprites_.AddSprite("JeepNoreste20", {280, 345, 40, 36});
    carSprites_.AddSprite("JeepSureste45", {80, 304, 40, 40});
    carSprites_.AddSprite("JeepSuroeste45", {240, 304, 40, 40});
    carSprites_.AddSprite("JeepNoroeste45", {80, 344, 40, 40});
    carSprites_.AddSprite("JeepNoreste45", {240, 344, 40, 40});
    carSprites_.AddSprite("JeepSuroeste75", {201, 304, 36, 40});
    carSprites_.AddSprite("JeepSureste75", {123, 304, 36, 40});
    carSprites_.AddSprite("JeepNoreste75", {203, 344, 36, 40});
    carSprites_.AddSprite("JeepNoroeste75", {121, 344, 36, 40});
}

SpriteSheet& ResourceLoader::GetCarSprites() { return carSprites_; }
