#pragma once

#include "../../common.h"
#include "../../ui/credits.h"
#include "../opengl/texture.h"
#include <SDL_ttf.h>

typedef struct CreditsRenderer {
    Texture* lineTextures[MAX_CREDIT_LINES];
    Texture* endLineTexture;
} CreditsRenderer;

CreditsRenderer* createCreditsRenderer();
void initCreditsRenderer(Credits* credits, Graphics* gfx);
void renderCredits(Graphics* gfx, Credits* credits);
void destroyCreditsRenderer(CreditsRenderer* creditsRenderer);

