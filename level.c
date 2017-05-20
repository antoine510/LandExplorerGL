#include "level.h"
#include "physics/worldConstraints.h"
#include "ai/AI.h"
#include "terrain/terrainGeneration.h"
#include "mathUtility.h"
#include "sound.h"
#include "entityModels.h"
#include "terrain/heightProfile.h"

#define LEVEL_END_ZONE 4070

static void endTriggerFun(void* level);

Level* initLevel(SDL_Point* viewOrigin)
{
    Level* level = malloc(sizeof(Level));
    level->terrain = initTerrain();
    level->entities = initEntities();
    level->waterManager = waterman_create();
    level->skyColor = (SDL_Color){192, 192, 255, 0};

    level->playerID = getFreeEntity(level->entities);
    setEntityAsModel(level->entities, level->playerID, "player");
    level->spawnPoint = (Vect2){64.0f, OCEAN_HEIGHT*level->terrain->height - level->entities->collData[level->playerID].sz.y -1.0f};

    generateTerrain(level);

    int guardID = getFreeEntity(level->entities);
    setEntityAsModel(level->entities, guardID, "archer");
    setCollisionPos(level->entities, guardID, (Vect2){94.0f, OCEAN_HEIGHT*level->terrain->height - level->entities->collData[guardID].sz.y -20.0f});
    level->entities->aiData[guardID].playerID = level->playerID;

    int endTriggerID = getFreeEntity(level->entities);
    setEntityAsModel(level->entities, endTriggerID, "endBoat");
    level->entities->triggerData[endTriggerID].data = (void*)level;
    level->entities->triggerData[endTriggerID].trigger = endTriggerFun;
    setCollisionPos(level->entities, endTriggerID, (Vect2){level->terrain->width - TERRAIN_BORDER - 40.0f, OCEAN_HEIGHT*level->terrain->height - level->entities->collData[endTriggerID].sz.y});

    setCollisionPos(level->entities, level->playerID, level->spawnPoint);

    level->timeMinutes = 360.0f;

    level->playerControl = initPlayerControl(viewOrigin, level->entities, level->playerID);
    level->finished = false;

    return level;
}

ModeUpdateResult updateLevel(Level* level, KeyStates* keyStates)
{
    if(isKeyHeld(keyStates, key_map)) return MURESULT_MAP;
    if(isKeyHeld(keyStates, key_editor)) return MURESULT_EDITOR;

    static int counter = 0;

    updateController(keyStates, level, level->playerID, level->playerControl);
    if((counter % (FPS/AISPS)) == 0) ai_update(level->entities, level->terrain);
    #ifdef WATERMAN
    if((counter % (FPS/WSPS)) == 0) waterman_update(level->waterManager, level->terrain);
    #endif // WATERMAN
    updateEntities(level->entities, level->playerControl);
    applyConstraints(level->terrain, level->entities);

    if(level->finished)
    {
        soundstack_changeMusic(MUSIC_WIN);
        return MURESULT_WIN;
    }

    level->timeMinutes += 1.0f / PCPS;
    loopf(&level->timeMinutes, 1440.0);

    level->skyColor = getSkyColor(level->timeMinutes);

    counter++;

    return MURESULT_LEVEL;
}

void endTriggerFun(void* level)
{
    ((Level*)level)->finished = true;
}

SDL_Color getSkyColor(float timeMinutes)
{
    float brightness = 1.5f * sinf(timeMinutes /720 *PI) + 0.5f;
    clampf(&brightness, 0.0f, 1.0f);
    return (SDL_Color){50 + (Uint8)(205*brightness), 76 + (Uint8)(179*brightness), 128 + (Uint8)(127*brightness), 0};
}

void destroyLevel(Level* level)
{
    waterman_destroy(level->waterManager);
    destroyEntities(level->entities);
    destroyTerrain(level->terrain);
    destroyPlayerControl(level->playerControl);
}
