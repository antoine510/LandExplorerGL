#include "creditsRenderer.h"
#include "../graphics.h"

CreditsRenderer* createCreditsRenderer()
{
    CreditsRenderer* creditsRenderer = calloc(1, sizeof(CreditsRenderer));


    return creditsRenderer;
}

void initCreditsRenderer(Credits* credits, Graphics* gfx)
{
    SDL_Color textColor = {255, 255, 255, 255};
    int i;
    for(i = 0; i < credits->lineCount; i++)
    {
        TTF_Font* font = TTF_OpenFont("txt.ttf", credits->lineSize[i]);
        SDL_Surface* t = TTF_RenderText_Blended(font, credits->lines[i], textColor);
        gfx->creditsRenderer->lineTextures[i] = createTextureFromSurface(gfx->renderer, t);
		setTextureOriginRatio(gfx->creditsRenderer->lineTextures[i], 0.5f, 1.0f);
        SDL_FreeSurface(t);
        TTF_CloseFont(font);
    }

    TTF_Font* font = TTF_OpenFont("txt.ttf", credits->endLineSize);
    SDL_Surface* t = TTF_RenderText_Blended(font, credits->endLine, textColor);
    gfx->creditsRenderer->endLineTexture = createTextureFromSurface(gfx->renderer, t);
	setTextureOriginRatio(gfx->creditsRenderer->endLineTexture, 0.5f, 0.5f);
    setTexturePos(gfx->creditsRenderer->endLineTexture, 0, 0);
    SDL_FreeSurface(t);
    TTF_CloseFont(font);
}

void renderCredits(Graphics* gfx, Credits* credits)
{
    startFrameTexturesRendering(gfx->renderer);
        renderBackground(gfx->bgRenderer, gfx, BG_WIN);

        int y = (int)credits->baseY;
        int i;
        for(i = 0; i < MAX_CREDIT_LINES; i++)
        {
            if(gfx->creditsRenderer->lineTextures[i] != NULL)
            {
                setTexturePos(gfx->creditsRenderer->lineTextures[i], 0, y);
                drawTexture(gfx->creditsRenderer->lineTextures[i]);
                y -= gfx->creditsRenderer->lineTextures[i]->h + 5;
            }
            else break;
        }

        if(y < 0)
        {
            drawTexture(gfx->creditsRenderer->endLineTexture);
        }
    endFrameTexturesRendering();
}

void destroyCreditsRenderer(CreditsRenderer* creditsRenderer)
{
    int i;
    for(i = 0; i < MAX_CREDIT_LINES; i++)
    {
        if(creditsRenderer->lineTextures[i] != NULL) destroyTexture(creditsRenderer->lineTextures[i]);
    }
    if(creditsRenderer->endLineTexture != NULL) destroyTexture(creditsRenderer->endLineTexture);
    free(creditsRenderer);
}

