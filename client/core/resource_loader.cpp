#include "./resource_loader.h"

ResourceLoader::ResourceLoader(SDL2pp::Renderer& renderer):
        carSprites_(renderer, "../client/assets/need-for-speed/cars/cars.png") {
    LoadCarSprites();
}

void ResourceLoader::LoadCarSprites() {
    carSprites_.AddSprite("CamionetaRojaSur", {167, 304, 26, 39});
    carSprites_.AddSprite("CamionetaRojaNorte", {167, 345, 26, 39});
    carSprites_.AddSprite("CamionetaRojaEste", {0, 311, 39, 26});
    carSprites_.AddSprite("CamionetaRojaOeste", {1, 351, 39, 26});
    carSprites_.AddSprite("CamionetaRojaSureste20", {40, 307, 40, 36});
    carSprites_.AddSprite("CamionetaRojaSuroeste20", {280, 307, 40, 36});
    carSprites_.AddSprite("CamionetaRojaNoroeste20", {40, 345, 40, 36});
    carSprites_.AddSprite("CamionetaRojaNoreste20", {280, 345, 40, 36});
    carSprites_.AddSprite("CamionetaRojaSureste45", {80, 304, 40, 40});
    carSprites_.AddSprite("CamionetaRojaSuroeste45", {240, 304, 40, 40});
    carSprites_.AddSprite("CamionetaRojaNoroeste45", {80, 344, 40, 40});
    carSprites_.AddSprite("CamionetaRojaNoreste45", {240, 344, 40, 40});
    carSprites_.AddSprite("CamionetaRojaSuroeste75", {201, 304, 36, 40});
    carSprites_.AddSprite("CamionetaRojaSureste75", {123, 304, 36, 40});
    carSprites_.AddSprite("CamionetaRojaNoreste75", {203, 344, 36, 40});
    carSprites_.AddSprite("CamionetaRojaNoroeste75", {121, 344, 36, 40});
}

SpriteSheet& ResourceLoader::GetCarSprites() { return carSprites_; }
