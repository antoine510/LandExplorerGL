#include "backgroundRenderer.h"
#include "../../mathUtility.h"
#include "../../terrain/terrainGeneration.h"
#include "../graphics.h"

static void renderSun(BackgroundRenderer* bgRenderer, float time);

BackgroundRenderer* createBackgroundRenderer()
{
    BackgroundRenderer* bgRenderer = calloc(1, sizeof(BackgroundRenderer));

    return bgRenderer;
}

void backgroundRendererLoadTextures(BackgroundRenderer* bgRenderer, OGLRenderer* renderer, SDL_Color* skyColor, xmlNodePtr mainNode)
{
    xmlNodePtr texture = mainNode->children;
    while(texture->type == XML_TEXT_NODE) texture = texture->next;

	while (texture)
    {
        char fileName[MAX_FILENAME_SIZE] = "textures/";
        strcat(fileName, asStringl(mainNode, "path"));
        strcat(fileName, asStringl(texture, "path"));
        if(checkName(texture, "background")) {
            int index = asIntl(texture, "id");
            bgRenderer->background[index] = createTextureFromFile(renderer, fileName);
            if(index == BG_BIOMES + BIOME_PLAINS || index == BG_BIOMES + BIOME_MOUNTAINS || index == BG_BIOMES + BIOME_OCEAN)
            {
                setTextureColorMod(bgRenderer->background[index], skyColor);
				setTextureClipSize(bgRenderer->background[index], TERRAIN_WIDTH * BLOC_SIZE / 2, bgRenderer->background[index]->h);
				setTextureScale(bgRenderer->background[index], 2.5f);
                setTexutreScalingMethod(bgRenderer->background[index], GL_LINEAR);
            }
            else
            {
                setTextureFullscreenCrop(bgRenderer->background[index]);
            }
        } else if (checkName(texture, "sun")) {
		    bgRenderer->sun = createTextureFromFile(renderer, fileName);
        }
        do texture = texture->next; while(texture && texture->type == XML_TEXT_NODE);
	}
}

void setBackgroundRendererTime(BackgroundRenderer* bgRenderer, float* levelTime)
{
    bgRenderer->levelTime = levelTime;
}

void renderBackground(BackgroundRenderer* bgRenderer, Graphics* gfx, int bgID)
{
    SDL_Color bgColor = modulateColor(&gfx->texPack->skyColor, 0.77f, 0.9f, 1.0f);
    clearScreen(&bgColor);
    if(bgID == BG_BIOMES + BIOME_PLAINS)
    {
        renderSun(bgRenderer, *bgRenderer->levelTime);
        setTexturePos(bgRenderer->background[bgID], -gfx->viewOrigin.x / 2, -4500 + gfx->viewOrigin.y/2);
    }
    else if(bgID == BG_BIOMES + BIOME_MOUNTAINS)
    {
        renderSun(bgRenderer, *bgRenderer->levelTime);
        setTexturePos(bgRenderer->background[bgID], -gfx->viewOrigin.x / 2, -2500 + gfx->viewOrigin.y/2);
    }
    else if(bgID == BG_BIOMES + BIOME_OCEAN)
    {
        renderSun(bgRenderer, *bgRenderer->levelTime);
        setTexturePos(bgRenderer->background[bgID], -gfx->viewOrigin.x / 2, -4900 + gfx->viewOrigin.y/2);
    }
    drawTexture(bgRenderer->background[bgID]);
}

void renderSun(BackgroundRenderer* bgRenderer, float time)
{
    if(time > 0.0 && time < 720.0)
    {
        int x = (int)((0.75f - cosf(time / 1440 * PIx2)) * myDisplayMode->w * 2 /3);
        int y = (int)((1 - sinf(time / 1440 * PIx2)) * myDisplayMode->h/2);
        setTexturePos(bgRenderer->sun, x, y);
        drawTexture(bgRenderer->sun);
    }
}

void destroyBackgroundRenderer(BackgroundRenderer* bgRenderer)
{
    int i;
    for(i = 0; i < MAX_BACKGROUND_COUNT; i++)
    {
        if(bgRenderer->background[i] != NULL) destroyTexture(bgRenderer->background[i]);
    }
    if(bgRenderer->sun != NULL) destroyTexture(bgRenderer->sun);
    free(bgRenderer);
}
