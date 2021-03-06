#include "entities.h"
#include "entityModels.h"
#include "xmlTools.h"
#include "physics/collisions.h"
#include "player/playerController.h"
#include "mathUtility.h"

static void updateWeapon(Entities* entities, int id, PlayerControl* pControl);

Entities* initEntities()
{
    Entities* entities = calloc(1, sizeof(Entities));
    entities->sap = sap_create();

    return entities;
}

void initCollisionData(Entities* entities, unsigned int id)
{
    entities->collData[id].SAPClientID = sap_addClient(entities->sap, id, entities->collData[id].pos, entities->collData[id].sz);
}

void setCollisionPos(Entities* entities, int id, Vect2 pos)
{
    entities->collData[id].pos = pos;
    entities->rect[id].x = (int)(pos.x * BLOC_SIZE);
    entities->rect[id].y = (int)(pos.y * BLOC_SIZE);
}

void deleteCollisionData(CollisionData* collData, Entities* entities)
{
    sap_removeClient(entities->sap, collData->SAPClientID);
}

unsigned int getFreeEntity(Entities* entities)
{
    int i;
    for(i = 0; i < ENTITY_COUNT; i++)
    {
        if(!entities->components[i]) return i;
    }
    printf("Error : Not enough entity slots");
    return 0;
}

void destroyEntity(Entities* entities, unsigned int id)
{
    if(entities->components[id] & COMP_STATUS) deleteStatus(entities->status[id]);
    if(entities->components[id] & COMP_COLLIDEABLE) deleteCollisionData(&entities->collData[id], entities);

    memset(&entities->components[id], 0, sizeof(Uint32));
    memset(&entities->rect[id], 0, sizeof(SDL_Rect));
    memset(&entities->gfxData[id], 0, sizeof(GfxData));
    memset(&entities->status[id], 0, sizeof(Status));
    memset(&entities->dynData[id], 0, sizeof(DynamicData));
    memset(&entities->collData[id], 0, sizeof(CollisionData));
    memset(&entities->aiData[id], 0, sizeof(AIData));
    memset(&entities->weaponData[id], 0, sizeof(WeaponData));
}

void updateEntities(Entities* entities, PlayerControl* pControl)
{
    int i;
    Uint32 target = COMP_COLLIDEABLE, target2 = COMP_WEAPON;
    for(i = 0; i < ENTITY_COUNT; i++)
    {
        if((entities->components[i] & target) == target)
        {
            if((entities->components[i] & target2) == target2)
            {
                updateWeapon(entities, i, pControl);
            }
            sap_updateClient(entities->sap, entities->collData[i].SAPClientID, entities->collData[i].pos, entities->collData[i].sz);
            entities->collData[i].collTime++;
        }
    }
    sap_sortPoints(entities->sap);
}

void updateWeapon(Entities* entities, int id, PlayerControl* pControl)
{
    if(entities->weaponData[id].type == weapon_sword)
    {
        int parentID = entities->weaponData[id].parentID;
        entities->weaponData[id].lifetime++;
        float centerX = entities->collData[parentID].pos.x + entities->collData[parentID].sz.x / 2;
        float centerY = entities->collData[parentID].pos.y + entities->collData[parentID].sz.y / 3;
		float angle = pControl->direction * 240.0f * entities->weaponData[id].lifetime / PCPS;

        float x = cosf(angle * DEG2RAD)*3.0f + centerX, y = sinf(angle * DEG2RAD)*3.0f + centerY;
	
        entities->collData[id].pos.x = x;
        entities->collData[id].pos.y = y;
		entities->gfxData[id].angle = -angle +45.0f;
        entities->rect[id].x = (int)(centerX*BLOC_SIZE);
        entities->rect[id].y = (int)(centerY*BLOC_SIZE);
        if(entities->weaponData[id].lifetime > 0.5f * PCPS)
        {
            destroyEntity(entities, id);
            pControl->currentWeaponID = -1;
        }
    } else if (entities->weaponData[id].type == weapon_arrow) {
        Vect2 v = vect2_normalize(entities->dynData[id].velocity);

        float angle = (v.y >= 0) ? 360.0f - acosf(v.x)*RAD2DEG : acosf(v.x)*RAD2DEG;

        entities->gfxData[id].angle = angle - 90.0f;
    }
}

void spawnArrow(Entities* entities, int parentID, Vect2 velocity, Vect2 parentOffset)
{
    unsigned int entityID = getFreeEntity(entities);
    setEntityAsModel(entities, entityID, "arrow");
	addTo(&parentOffset, entities->collData[parentID].pos);
    entities->collData[entityID].pos = parentOffset;
    entities->dynData[entityID].direction = entities->dynData[parentID].direction;
	multiplyBy(&velocity, entities->weaponData[entityID].velocity);
	entities->dynData[entityID].velocity = velocity;
    entities->weaponData[entityID].parentID = parentID;
}

void destroyEntities(Entities* entities)
{
    int i;
    for(i = 0; i < ENTITY_COUNT; i++)
    {
        destroyEntity(entities, i);
    }
    sap_destroy(entities->sap);
    free(entities);
}
