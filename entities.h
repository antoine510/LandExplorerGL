#pragma once

#include "common.h"
#include "components.h"
#include "physics/sweepAndPrune.h"

#define ENTITY_COUNT 128

typedef struct Entities {
    SAP* sap;

    Uint32 components[ENTITY_COUNT];
    SDL_Rect rect[ENTITY_COUNT];
    GfxData gfxData[ENTITY_COUNT];
    Status status[ENTITY_COUNT];
    DynamicData dynData[ENTITY_COUNT];
    CollisionData collData[ENTITY_COUNT];
    AIData aiData[ENTITY_COUNT];
    WeaponData weaponData[ENTITY_COUNT];
    TriggerData triggerData[ENTITY_COUNT];
} Entities;

Entities* initEntities();

void initCollisionData(Entities* entities, unsigned int id);
void setCollisionPos(Entities* entities, int id, Vect2 pos);
void deleteCollisionData(CollisionData* collData, Entities* entities);

void spawnArrow(Entities* entities, int parentID, Vect2 velocity, Vect2 parentOffset);

unsigned int getFreeEntity(Entities* entities);
void destroyEntity(Entities* entities, unsigned int id);

void updateEntities(Entities* entities, PlayerControl* pControl);

void destroyEntities(Entities* entities);

