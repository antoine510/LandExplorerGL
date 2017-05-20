#include "graphics.h"
#include "../mathUtility.h"
#include "../terrain/blockTypes.h"

static void renderEntities(Graphics* gfx, Entities* entities);
static void initRenderers(Graphics* gfx);
static void destroyRenderers(Graphics* gfx);

SDL_DisplayMode* myDisplayMode;

Graphics* initGraphics()
{
    Graphics* gfx = malloc(sizeof(Graphics));
    SDL_assert_release(gfx != NULL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    gfx->window = SDL_CreateWindow("Land Explorer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_assert_release(gfx->window != NULL);

    myDisplayMode = malloc(sizeof(SDL_DisplayMode));
    SDL_GetWindowDisplayMode(gfx->window, myDisplayMode);

    gfx->oglContext = SDL_GL_CreateContext(gfx->window);
    SDL_assert_release(gfx->oglContext != NULL);

    SDL_GL_SetSwapInterval(0);

    int major, minor;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    printf("Using OpenGL version %d.%d\n\n", major, minor);

    #ifdef WIN32
    GLenum err = glewInit();
    SDL_assert_release(err == GLEW_OK);
    #endif

    gfx->renderer = createOGLRenderer(gfx->window);
    if(gfx->renderer == NULL) fprintf(stderr, "Erreur : OpenGL renderer creation failed");

    initRenderers(gfx);

    gfx->viewOrigin.x = 0;
    gfx->viewOrigin.y = 0;
    gfx->playerRect = NULL;

    return gfx;
}

void initRenderers(Graphics* gfx)
{
    gfx->creditsRenderer = createCreditsRenderer();
    gfx->playerInterfaceRenderer = createPlayerInterfaceRenderer(gfx);
    gfx->bgRenderer = createBackgroundRenderer();
    initMenuRenderer(&gfx->menuRenderer);
    initMapRenderer(&gfx->mapRenderer, gfx);
    gfx->texPack = initTexturePack(gfx);
    chunckRenderer = initChunckRenderer(gfx->texPack, &gfx->viewOrigin);
    initEditorRenderer(&gfx->editorRenderer, gfx);
}

void setDisplaySize(Graphics* gfx, int w, int h)
{
    setDisplayFullscreen(gfx, false);
    if(w != myDisplayMode->w || h != myDisplayMode->h)
    {
        SDL_SetWindowSize(gfx->window, w, h);
        SDL_GetWindowDisplayMode(gfx->window, myDisplayMode);
        glViewport(0, 0, w, h);
        destroyRenderers(gfx);
        initRenderers(gfx);
    }
}

void toggleDisplayFullscreen(Graphics* gfx)
{
    bool fullscreen = !(SDL_GetWindowFlags(gfx->window) & SDL_WINDOW_FULLSCREEN_DESKTOP);
    setDisplayFullscreen(gfx, fullscreen);
}

void setDisplayFullscreen(Graphics* gfx, bool fullscreen)
{
    if(fullscreen)
    {
        SDL_DisplayMode mode;
        SDL_GetDesktopDisplayMode(0, &mode);
        setDisplaySize(gfx, mode.w, mode.h);
    }
    SDL_SetWindowFullscreen(gfx->window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void renderLevel(Graphics* gfx, Level* level)
{
    gfx->texPack->skyColor = level->skyColor;

    startFrameTexturesRendering(gfx->renderer);
        Vect2 playerPos = level->entities->collData[level->playerID].pos;
        int biomeID = getChunckPtr(level->terrain, (Uint32)(playerPos.x/CHUNCK_WIDTH), (Uint32)(playerPos.y/CHUNCK_HEIGHT))->biome;
        renderBackground(gfx->bgRenderer, gfx, BG_BIOMES + biomeID);

        updateCamera(gfx);

        updateChunckRenderer(chunckRenderer);
        renderBackwallScreen(chunckRenderer);
        renderEntities(gfx, level->entities);
        renderBlocScreen(chunckRenderer);

        renderPlayerInterface(gfx, level->playerControl);
    endFrameTexturesRendering();
}

void initCamera(Graphics* gfx, Entities* entities)
{
    int i;
    Uint32 target = COMP_PLAYER_CONTROL + COMP_RECT;
    for(i = 0; i < ENTITY_COUNT; i++)
    {
        if((entities->components[i] & target) == target)
        {
            gfx->playerRect = &entities->rect[i];
            break;
        }
    }
    gfx->viewOrigin.x = gfx->playerRect->x + gfx->playerRect->w/2;
    gfx->viewOrigin.y = gfx->playerRect->y + gfx->playerRect->h/2;
    updateCamera(gfx);

    initMapRendererStartPos(&gfx->mapRenderer, gfx);
}

void updateCamera(Graphics* gfx)
{
    //gfx->viewOrigin.x = lerp(gfx->viewOrigin.x, gfx->playerRect->x + gfx->playerRect->w/2 - myDisplayMode->w/2, 0.5f);
    gfx->viewOrigin.x = gfx->playerRect->x + gfx->playerRect->w/2;
    gfx->viewOrigin.y = gfx->playerRect->y + gfx->playerRect->h / 2;//lerp(gfx->viewOrigin.y, gfx->playerRect->y + gfx->playerRect->h/2, 0.25f);

    //Violent movements should not be interpolated
    if(abs(gfx->viewOrigin.y - (gfx->playerRect->y + gfx->playerRect->h/2)) > 2*BLOC_SIZE)
        gfx->viewOrigin.y = gfx->playerRect->y + gfx->playerRect->h/2;		///ALWAYS USED !!! TODO

    //Border conditions
    if(gfx->viewOrigin.x - myDisplayMode->w / 2 < TERRAIN_BORDER*BLOC_SIZE)
        {gfx->viewOrigin.x = TERRAIN_BORDER*BLOC_SIZE + myDisplayMode->w / 2;}
    else if(gfx->viewOrigin.x + myDisplayMode->w/2 > (TERRAIN_WIDTH - TERRAIN_BORDER)*BLOC_SIZE)
        {gfx->viewOrigin.x = (TERRAIN_WIDTH - TERRAIN_BORDER)*BLOC_SIZE - myDisplayMode->w / 2;}

    if(gfx->viewOrigin.y - myDisplayMode->h / 2 < TERRAIN_BORDER*BLOC_SIZE)
        {gfx->viewOrigin.y = TERRAIN_BORDER*BLOC_SIZE + myDisplayMode->h / 2;}
    else if(gfx->viewOrigin.y + myDisplayMode->h/2 > (TERRAIN_HEIGHT - TERRAIN_BORDER)*BLOC_SIZE)
        {gfx->viewOrigin.y = (TERRAIN_HEIGHT - TERRAIN_BORDER)*BLOC_SIZE - myDisplayMode->h/2;}
}

void renderEntities(Graphics* gfx, Entities* entities)
{
    int i;
    Uint32 target = COMP_GFXDATA + COMP_RECT;
    for(i = 0; i < ENTITY_COUNT; i++)
    {
        if((entities->components[i] & target) == target)
        {
            SDL_Rect t = entities->rect[i];
            t.x -= gfx->viewOrigin.x;
            t.y = gfx->viewOrigin.y - t.y - t.h;
            blitEntity(gfx->texPack, &entities->gfxData[i], t);
        }
    }
    texPackFrameReset(gfx->texPack);
}

void updateScreen(Graphics* gfx)
{
    SDL_GL_SwapWindow(gfx->window);
}

void destroyRenderers(Graphics* gfx)
{
    destroyEditorRenderer(&gfx->editorRenderer);
    destroyChunckRenderer(chunckRenderer);
    destroyTexturePack(gfx->texPack);
    destroyMapRenderer(&gfx->mapRenderer);
    destroyMenuRenderer(&gfx->menuRenderer);
    destroyBackgroundRenderer(gfx->bgRenderer);
    destroyPlayerInterfaceRenderer(gfx->playerInterfaceRenderer);
    destroyCreditsRenderer(gfx->creditsRenderer);
}

void destroyGraphics(Graphics* gfx)
{
    destroyRenderers(gfx);

    destroyOGLRenderer(gfx->renderer);

    free(myDisplayMode);

    SDL_GL_DeleteContext(gfx->oglContext);
    SDL_DestroyWindow(gfx->window);

    free(gfx);
}

