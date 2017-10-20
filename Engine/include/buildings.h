#ifndef __BUILDINGS_H__
#define __BUILDINGS_H__

#define MAX_BUILDINGS 50

typedef struct Building_S
{
	enum{Empty, UnderConstruction, Apartment, Elevator} buildingType;
	short inUse;
	Vector2D size;
	Vector2D position;
	Vector2D scale;
	Sprite* sprite;
	SDL_Rect bounding_box;
	Vector4D color;

	void(*update)(struct Entity_S *self);
	void(*free)(struct Entity_S *self);
}Building;

/**
* @brief reserve memory for building list and set it to 0
*/
void clearBuildingList();

/**
* @brief find a next available spot in the list, set it's in use to 1, return the pointer
* @return Entity*
*/
Building* building_new();

/**
* @brief setup all variables of a character entity
* @param Entity*	Pointer to building object to be set up
* @return Entity*
*/
Building* building_setup(Building* e, int type);

/**
* @brief free entity
* @param Entity**	Double pointer to Entity you want to free
*/
void building_free(Building** e);

/**
* @brief update entity
* @param Entity*	Pointer to Entity you want to update
*/
void building_update(Building* e);

/**
* @brief go thru building list and update all
*/
void update_buildings();

/**
* @brief set position of Building e
* @param e*		pointer to entity
* @param int x  position to set building position x
* @param int y  position to set building position y
*/
void building_set_position(Building* e, int x, int y);

/**
* @brief count current number of buildings
*/
int building_count();

#endif // !__BUILDINGS_H__

