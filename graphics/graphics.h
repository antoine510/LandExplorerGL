#pragma once

#include "../common.h"
#include "texturePack.h"
#include "chunkRenderer.h"
#include "../level.h"
#include "../entities.h"
#include "renderers/editorRenderer.h"
#include "renderers/mapRenderer.h"
#include "renderers/menuRenderer.h"
#include "renderers/backgroundRenderer.h"
#include "renderers/creditsRenderer.h"
#include "renderers/playerInterfaceRenderer.h"
#include "opengl/renderer.h"
#include "displayInfo.h"

struct Graphics {
    OGLRenderer* renderer;
    SDL_Window* window;
    SDL_GLContext oglContext;

    TexturePack* texPack;
    EditorRenderer editorRenderer;
    MapRenderer mapRenderer;
    MenuRenderer menuRenderer;
    BackgroundRenderer* bgRenderer;
    CreditsRenderer* creditsRenderer;
    PlayerInterfaceRenderer* playerInterfaceRenderer;

    SDL_Point viewOrigin;
    SDL_Rect* playerRect;
    Texture* deathTextTexture;
};

Graphics* initGraphics();
void setDisplaySize(Graphics* gfx, int w, int h);
void toggleDisplayFullscreen(Graphics* gfx);
void setDisplayFullscreen(Graphics* gfx, bool fullscreen);

void renderLevel(Graphics* gfx, Level* level);

void initCamera(Graphics* gfx, Entities* entities);
void updateCamera(Graphics* gfx);

void updateScreen(Graphics* gfx);

void destroyGraphics(Graphics* gfx);

