#pragma once

#include "../../common.h"
#include "../../editor.h"
#include "../opengl/texture.h"
#include <SDL_ttf.h>

typedef struct EditorRenderer {
    TTF_Font* font;

    Texture* moduleTextTexture;
    Texture* modeNameTexture;

    Texture* selectRefTex, *selectModuleTex, *selectArcherTex, *selectGuardTex, *bgOverlay;
} EditorRenderer;

void initEditorRenderer(EditorRenderer* editorRenderer, Graphics* gfx);
void renderEditor(Graphics* gfx, Editor* editor);
void destroyEditorRenderer(EditorRenderer* editorRenderer);

