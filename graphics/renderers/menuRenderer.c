#include "menuRenderer.h"
#include "../graphics.h"

#define TITLE_FONT_SIZE 80
#define BUTTON_FONT_SIZE 35
#define SELECTED_BUTTON_FONT_SIZE 45

static void updateMenuTextures(MenuRenderer* menuRenderer, Graphics* gfx, Menu* menu);
static void destroyMenuTextures(MenuRenderer* menuRenderer);
static SliderTextures createSliderTextures(Graphics* gfx, Slider* slider, TTF_Font* font, SDL_Color color);

void menuRendererLoadTextures(MenuRenderer* menuRenderer, OGLRenderer* renderer, xmlNodePtr mainNode)
{
    xmlNodePtr texture = mainNode->children;
    while(texture->type == XML_TEXT_NODE) texture = texture->next;

	while (texture)
    {
        char fileName[MAX_FILENAME_SIZE] = "textures/";
        strcat(fileName, asStringl(mainNode, "path"));
        strcat(fileName, asStringl(texture, "path"));
        if(checkName(texture, "outline")) {
            menuRenderer->outline = createTextureFromFile(renderer, fileName);
			setTextureOriginRatio(menuRenderer->outline, 0.0f, 1.0f);
            setTexturePos(menuRenderer->outline, -myDisplayMode->w / 2 + 10, myDisplayMode->w / 2 - 10);
        }
        do texture = texture->next; while(texture && texture->type == XML_TEXT_NODE);
	}
}

void initMenuRenderer(MenuRenderer* menuRenderer)
{
    memset(menuRenderer, 0, sizeof(MenuRenderer));

    menuRenderer->titleFont = TTF_OpenFont("txt.ttf", TITLE_FONT_SIZE);
    menuRenderer->buttonFont = TTF_OpenFont("txt.ttf", BUTTON_FONT_SIZE);
    menuRenderer->selectedButtonFont = TTF_OpenFont("txt.ttf", SELECTED_BUTTON_FONT_SIZE);

    menuRenderer->titleColor = (SDL_Color){249, 214, 91, 255};
    menuRenderer->buttonColor = (SDL_Color){240, 240, 240, 255};
    menuRenderer->selectedButtonColor = (SDL_Color){255, 255, 180, 255};
    menuRenderer->labelColor = (SDL_Color){255, 255, 50, 255};
}

void renderMenu(Graphics* gfx, Menu* menu)
{
    if(menu->stateChanged)
    {
        updateMenuTextures(&gfx->menuRenderer, gfx, menu);
        menu->stateChanged = false;
    }

    startFrameTexturesRendering(gfx->renderer);
        renderBackground(gfx->bgRenderer, gfx, BG_MENU);

        drawTexture(gfx->menuRenderer.outline);
        drawTexture(gfx->menuRenderer.titleTexture);
        int i;
        for(i = 0; i < menu->subMenus[menu->currentSubMenu].buttonCount; i++)
        {
            if(i != menu->subMenus[menu->currentSubMenu].selection) drawTexture(gfx->menuRenderer.buttonTexture[i]);
        }
        for(i = 0; i < menu->subMenus[menu->currentSubMenu].labelCount; i++)
        {
            drawTexture(gfx->menuRenderer.labelTexture[i]);
        }
        for(i = 0; i < MAX_MENU_ELEMENT_COUNT; i++)
        {
            if(gfx->menuRenderer.sliderTextures[i].nameTexture != NULL)
            {
                drawTexture(gfx->menuRenderer.sliderTextures[i].nameTexture);
                drawTexture(gfx->menuRenderer.sliderTextures[i].valueTexture);
            }
        }
        if(gfx->menuRenderer.selectedButtonTexture != NULL) drawTexture(gfx->menuRenderer.selectedButtonTexture);
    endFrameTexturesRendering();
}

void updateMenuTextures(MenuRenderer* menuRenderer, Graphics* gfx, Menu* menu)
{
    SubMenu* subMenu = &menu->subMenus[menu->currentSubMenu];

    destroyMenuTextures(menuRenderer);

    SDL_Surface* t = TTF_RenderText_Blended(menuRenderer->titleFont, subMenu->name, menuRenderer->titleColor);
    menuRenderer->titleTexture = createTextureFromSurface(gfx->renderer, t);
	setTextureOriginRatio(menuRenderer->titleTexture, 0.0f, 1.0f);
    SDL_FreeSurface(t);
    setTexturePos(menuRenderer->titleTexture, -myDisplayMode->w/2 + 25, myDisplayMode->h/2 - 20);

    int i;
    for(i = 0; i < subMenu->buttonCount; i++)
    {
        t = TTF_RenderText_Blended(menuRenderer->buttonFont, subMenu->buttons[i]->name, menuRenderer->buttonColor);
        menuRenderer->buttonTexture[i] = createTextureFromSurface(gfx->renderer, t);
		setTextureOriginRatio(menuRenderer->buttonTexture[i], 0.0f, 0.5f);
        SDL_FreeSurface(t);
        setTexturePos(menuRenderer->buttonTexture[i], subMenu->buttons[i]->rect.x, subMenu->buttons[i]->rect.y);
    }
    for(i = 0; i < subMenu->labelCount; i++)
    {
        t = TTF_RenderText_Blended(menuRenderer->buttonFont, subMenu->labels[i].text, menuRenderer->labelColor);
        menuRenderer->labelTexture[i] = createTextureFromSurface(gfx->renderer, t);
		setTextureOriginRatio(menuRenderer->labelTexture[i], 0.0f, 0.5f);
        SDL_FreeSurface(t);
        setTexturePos(menuRenderer->labelTexture[i], subMenu->labels[i].pos.x, subMenu->labels[i].pos.y);
    }
    for(i = 0; i < MAX_MENU_ELEMENT_COUNT; i++)
    {
        if(subMenu->sliders[i] != NULL)
        {
            menuRenderer->sliderTextures[i] = createSliderTextures(gfx, subMenu->sliders[i], menuRenderer->buttonFont, menuRenderer->buttonColor);
        }
    }

    t = TTF_RenderText_Blended(menuRenderer->selectedButtonFont, subMenu->buttons[subMenu->selection]->name, menuRenderer->selectedButtonColor);
    menuRenderer->selectedButtonTexture = createTextureFromSurface(gfx->renderer, t);
	setTextureOriginRatio(menuRenderer->selectedButtonTexture, 0.0f, 0.5f);
    SDL_FreeSurface(t);
    setTexturePos(menuRenderer->selectedButtonTexture,
                          subMenu->buttons[subMenu->selection]->rect.x,
                          subMenu->buttons[subMenu->selection]->rect.y);
}

SliderTextures createSliderTextures(Graphics* gfx, Slider* slider, TTF_Font* font, SDL_Color color)
{
    SliderTextures tex;
    SDL_Surface* t = TTF_RenderText_Blended(font, slider->name, color);
    tex.nameTexture = createTextureFromSurface(gfx->renderer, t);
	setTextureOriginRatio(tex.nameTexture, 0.0f, 0.5f);
    SDL_FreeSurface(t);
    setTexturePos(tex.nameTexture, slider->pos.x, slider->pos.y);

    char numberText[10];
    sprintf(numberText, "%d", slider->value);
    t = TTF_RenderText_Blended(font, numberText, color);
    tex.valueTexture = createTextureFromSurface(gfx->renderer, t);
	setTextureOriginRatio(tex.valueTexture, 0.0f, 0.5f);
    SDL_FreeSurface(t);
    setTexturePos(tex.valueTexture, slider->pos.x +370, slider->pos.y);

    return tex;
}

void destroyMenuTextures(MenuRenderer* menuRenderer)
{
    if(menuRenderer->titleTexture != NULL) destroyTexture(menuRenderer->titleTexture);
    menuRenderer->titleTexture = NULL;
    int i;
    for(i = 0; i < MAX_MENU_ELEMENT_COUNT; i++)
    {
        if(menuRenderer->buttonTexture[i] != NULL) destroyTexture(menuRenderer->buttonTexture[i]);
        menuRenderer->buttonTexture[i] = NULL;

        if(menuRenderer->labelTexture[i] != NULL) destroyTexture(menuRenderer->labelTexture[i]);
        menuRenderer->labelTexture[i] = NULL;

        if(menuRenderer->sliderTextures[i].nameTexture != NULL)
        {
            destroyTexture(menuRenderer->sliderTextures[i].nameTexture);
            destroyTexture(menuRenderer->sliderTextures[i].valueTexture);
        }
        menuRenderer->sliderTextures[i].nameTexture = NULL;
    }
    if(menuRenderer->selectedButtonTexture != NULL) destroyTexture(menuRenderer->selectedButtonTexture);
    menuRenderer->selectedButtonTexture = NULL;
}

void destroyMenuRenderer(MenuRenderer* menuRenderer)
{
    destroyMenuTextures(menuRenderer);
    destroyTexture(menuRenderer->outline);

    TTF_CloseFont(menuRenderer->titleFont);
    TTF_CloseFont(menuRenderer->buttonFont);
    TTF_CloseFont(menuRenderer->selectedButtonFont);
}

