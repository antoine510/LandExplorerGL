#pragma once

#include "../../common.h"
#include "../opengl/texture.h"
#include "../../map.h"
#include "../../level.h"

typedef struct MapRenderer {
    Texture* mapp;
    SDL_Surface* pixels;
    Uint32 blockColors[512];

    Uint32 x, y, x2, y2;
} MapRenderer;

void initMapRenderer(MapRenderer* mapRenderer, Graphics* gfx);
void initMapRendererStartPos(MapRenderer* mapRenderer, Graphics* gfx);

void updateMapRendering(MapRenderer* mapRenderer, Graphics* gfx, Map* mapp, Terrain* terrain);
void renderMap(MapRenderer* mapRenderer, Graphics* gfx, Map* mapp, Level* level);

void destroyMapRenderer(MapRenderer* mapRenderer);

