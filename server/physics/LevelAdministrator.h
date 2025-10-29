#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <box2d/box2d.h>
#include <SDL.h>

class LevelAdministrator {
public:
    struct BuildConfig {
        float ppm = 2.0f;               // 2 px = 1 m  -> cada celda de 2px mide 1m
        bool flipY = false;             // invertir Y (si tu render usa origen abajo-izq)
        int levelPxHeight = 0;          // requerido si flipY=true (alto en px del nivel)
        float friction = 0.9f;
        float restitution = 0.0f;
        bool mergeHorizontal = true;    // agrupa runs horizontales en un solo fixture
    };

    // Carga la IntGrid de colisión desde .ldtk o .ldtkl
    // levelId: "" para el primero (si usás .ldtkl por-nivel, dejalo "")
    // layerId: nombre de tu capa IntGrid (ej: "Collision2px")
    bool loadCollisionGrid(const std::string& ldtkPath,
                           const std::string& levelId,
                           const std::string& layerId,
                           std::string* outErr = nullptr);

    // Construye un body estático con fixtures según la grilla cargada
    // Devuelve el body creado (o B2_NULL_BODY si falló o no hay grilla)
    b2BodyId buildCollision(b2WorldId world, const BuildConfig& cfg);

    // Accesos útiles
    int gridWidthCells()  const { return G_.w; }
    int gridHeightCells() const { return G_.h; }
    int gridSizePx()      const { return G_.gridPx; }
    bool hasGrid()        const { return !G_.v.empty(); }
    void debugDraw(SDL_Renderer* r,
                   float camX_px, float camY_px,
                   float zoom,
                   float screen_w, float screen_h,
                   SDL_Color color = {255,128,0,120}) const;
private:
    struct Grid {
        int w=0, h=0, gridPx=0;
        std::vector<uint8_t> v; // row-major: v[y*w + x], 0/1
    } G_;

    // Helpers de construcción
    b2BodyId buildCellByCell_(b2WorldId, const BuildConfig&);
    b2BodyId buildMergeHorizontal_(b2WorldId, const BuildConfig&);

    // Conversión (x,y celda) -> centro en metros
    inline void cellCenterMeters_(int x, int y, const BuildConfig& cfg,
                                  float cell_m, float& cx, float& cy) const;
};
