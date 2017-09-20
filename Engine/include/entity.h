#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_ENTITIES 100

typedef struct Entity_S
{
	short inUse;
	Vector2D position;
	Vector2D scale;
	Vector2D acceleration;
	Sprite* sprite;

	void(*update)(struct Entity_S *self);
}Entity;

extern Entity entList[MAX_ENTITIES];

/**
* @brief reserve memory for entity list and set it to 0
*/
void clearEntList();

/**
* @brief find a next available entity in the list, set it's in use to 1, return the pointer
* @return Entity* 
*/
Entity* entity_new();

/**
* @brief set in use to 0, set pointer to 0 
*/
//void entity_free(Entity** e);
void entity_free(Entity* e);

/**
* @brief update entity
*/
void entity_update(Entity* e);

/**
* @brief go thru entity list and update all entities
*/
void update_entities();

/**
* @brief set position of Entity e
* @param e pointer to entity
* @param x position to set entity position x
* @param y position to set entity position y
*/
void entity_set_position(Entity* e, int x, int y);

#endif // !__ENTITY_H__
