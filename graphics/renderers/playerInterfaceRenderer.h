#pragma once

#include "../../common.h"
#include "../opengl/texture.h"
#include "../../player/playerController.h"

typedef struct PlayerInterfaceRenderer {
    Texture* deathTextTexture;
} PlayerInterfaceRenderer;

PlayerInterfaceRenderer* createPlayerInterfaceRenderer(Graphics* gfx);
void renderPlayerInterface(Graphics* gfx, PlayerControl* pControl);
void destroyPlayerInterfaceRenderer(PlayerInterfaceRenderer* playerInterfaceRenderer);

