#ifndef __ENTITY_H__
#define __ENTITY_H__

#define MAX_ENTITIES 50
typedef enum {ATHOME, ATWORK, GOINGWORK, GOINGHOME} EntityState;

typedef struct Entity_S
{
	short inUse;
	Vector2D size;
	Vector2D position;
	Vector2D scale;
	Vector2D velocity;
	Sprite* sprite;
	SDL_Rect bounding_box;
	Building* home;
	Building* work;
	Building* destination;
	EntityState state;
	Building* currentBuilding;

	int state_time;
	int internal_time;
	int happiness;

	void(*update_e)(struct Entity_S *self);
	void(*free_e)(struct Entity_S *self);
}Entity;

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
* @brief setup all variables of a character entity
* @param Entity*	Pointer to character Entity to be set up 
* @return Entity*
*/
Entity* entity_setup_character(Entity* e);

/**
* @brief free entity
* @param Entity**	Double pointer to Entity you want to free
*/
void entity_free(Entity** e);

/**
* @brief update entity
* @param Entity*	Pointer to Entity you want to update
*/
void entity_update(Entity* e);

/**
* @brief go thru entity list and update all 
*/
void update_entities();

/**
* @brief set position of Entity e
* @param e*		pointer to entity
* @param int x  position to set entity position x
* @param int y  position to set entity position y
*/
void entity_set_position(Entity* e, int x, int y);

/**
* @brief set velocity of Entity e
* @param e*			pointer to entity
* @param float x	position to set entity position x
* @param float y	position to set entity position y
*/
void entity_set_velocity(Entity* e, float x, float y);

/**
* @brief count current number of entities
*/
int entity_count();

//docs
Entity* entity_set_home(Entity* e, Building* b);

int vec_to_vec(Vector2D this_vec, Vector2D other_vec);

Entity* entity_set_destination(Entity* e, Building* b);

Entity* entity_set_work(Entity* e, Building* b);

void set_state(Entity* e);
#endif // !__ENTITY_H__
