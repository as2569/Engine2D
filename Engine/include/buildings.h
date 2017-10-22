#ifndef __BUILDINGS_H__
#define __BUILDINGS_H__

#define MAX_BUILDINGS 50
typedef enum { EMPTY, CONSTRUCTION, APARTMENT, ELEVATOR } BuildingType;

typedef struct Building_S
{
	BuildingType buildingType;
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
* @return Building*
*/
Building* building_new();

/**
* @brief setup all variables of a character entity
* @param b	Pointer to building object to be set up
* @return Building*
*/
Building* building_setup(Building* b, int type);

/**
* @brief free entity
* @param b	Double pointer to Building you want to free
*/
void building_free(Building** b);

/**
* @brief update entity
* @param b	Pointer to Building you want to update
*/
void building_update(Building* b);

/**
* @brief go thru building list and update all
*/
void update_buildings();

/**
* @brief set position of Building b
* @param b*		pointer to Building
* @param x  position to set building position x
* @param y  position to set building position y
*/
void building_set_position(Building* b, int x, int y);

/**
* @brief count current number of buildings
* @return number of buildings
*/
int building_count();

/**
* @brief change under_construction block to valid block
* @param b	pointer to building that we want to change
*/
void construction_to_valid(Building* b);

#endif // !__BUILDINGS_H__

