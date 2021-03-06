#include "playerInterfaceRenderer.h"
#include "../graphics.h"

PlayerInterfaceRenderer* createPlayerInterfaceRenderer(Graphics* gfx)
{
    PlayerInterfaceRenderer* playerInterfaceRenderer = calloc(1, sizeof(PlayerInterfaceRenderer));

    SDL_Color color = {255, 255, 255, 255};
    TTF_Font* font = TTF_OpenFont("txt.ttf", 70);
    playerInterfaceRenderer->deathTextTexture = createTextureFromText(gfx->renderer, "You died !", 2, color, font);
	setTextureOriginRatio(playerInterfaceRenderer->deathTextTexture, 0.5f, 0.5f);
    setTexturePos(playerInterfaceRenderer->deathTextTexture, 0, 50);
    TTF_CloseFont(font);

    return playerInterfaceRenderer;
}

void renderPlayerInterface(Graphics* gfx, PlayerControl* pControl)
{
    if(pControl->breaking)
        blitBreak(gfx->texPack, pControl->breaking, pControl->breakingX*BLOC_SIZE - gfx->viewOrigin.x, gfx->viewOrigin.y - (pControl->breakingY+1)*BLOC_SIZE);
    int life = (int)(*pControl->playerLife);
    SDL_Point pos = {myDisplayMode->w/2 - 140, myDisplayMode->h / 2 - 50};
    while(life > HEALTH_PER_HEARTH)
    {
        blitHearth(gfx->texPack, pos, 1.0f);
        pos.x += 24;
        life -= HEALTH_PER_HEARTH;
    }
    blitHearth(gfx->texPack, pos, (float)life / HEALTH_PER_HEARTH);

    SDL_Point slotPos = { -myDisplayMode->w / 2 + 20, myDisplayMode->h / 2 - 50};
    int x, y, maxy = pControl->inventoryActive ? INVENTORY_HEIGHT : 1;
    for(y = 0; y < maxy; y++)
    {
        for(x = 0; x < INVENTORY_WIDTH; x++)
        {
            if(getSlotIndex(x, y) == pControl->inventory->selection)
            {
                slotPos.x -= 4; slotPos.y -= 4;
                blitSlot(gfx->texPack, slotPos, 1.25f);
                slotPos.x += 4; slotPos.y += 4;
            }
            else
            {
                blitSlot(gfx->texPack, slotPos, 1.0f);
            }
            if(pControl->inventory->itemCount[getSlotIndex(x, y)] > 0)
                blitItem(gfx->texPack, slotPos, pControl->inventory->itemID[getSlotIndex(x, y)]);
            slotPos.x += 40;
        }
        slotPos.x = -myDisplayMode->w / 2 + 20;
        slotPos.y -= 40;
    }
    if(pControl->cursorItemCount > 0)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        blitItem(gfx->texPack, (SDL_Point){ -myDisplayMode->w / 2 + x, myDisplayMode->h / 2 - y}, pControl->cursorItemID);
    }
    if(pControl->isDead) drawTexture(gfx->playerInterfaceRenderer->deathTextTexture);
}

void destroyPlayerInterfaceRenderer(PlayerInterfaceRenderer* playerInterfaceRenderer)
{
    if(playerInterfaceRenderer->deathTextTexture != NULL) destroyTexture(playerInterfaceRenderer->deathTextTexture);
    free(playerInterfaceRenderer);
}

