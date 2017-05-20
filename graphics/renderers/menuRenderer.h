#pragma once

#include "../../common.h"
#include "../../ui/menu.h"
#include "../opengl/texture.h"
#include "../../xmlTools.h"
#include <SDL_ttf.h>

typedef struct SliderTextures {
    Texture* nameTexture, *valueTexture;
} SliderTextures;

typedef struct MenuRenderer {
    TTF_Font* titleFont, *buttonFont, *selectedButtonFont;
    SDL_Color titleColor, buttonColor, selectedButtonColor, labelColor;

    Texture* titleTexture;
    Texture* buttonTexture[MAX_MENU_ELEMENT_COUNT];
    Texture* selectedButtonTexture;
    Texture* labelTexture[MAX_MENU_ELEMENT_COUNT];
    SliderTextures sliderTextures[MAX_MENU_ELEMENT_COUNT];
    Texture* outline;
} MenuRenderer;

void menuRendererLoadTextures(MenuRenderer* menuRenderer, OGLRenderer* renderer, xmlNodePtr mainNode);

void initMenuRenderer(MenuRenderer* menuRenderer);
void renderMenu(Graphics* gfx, Menu* menu);
void destroyMenuRenderer(MenuRenderer* menuRenderer);

