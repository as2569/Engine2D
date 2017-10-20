#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "def.h"
#include "buildings.h"

extern Building buildingList[MAX_BUILDINGS];

void clearBuildingList()
{
	memset(buildingList, 0, MAX_BUILDINGS * sizeof(Building));
}

Building* building_new()
{
	Building *e = NULL;
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (!buildingList[i].inUse)
		{
			Building *e = &buildingList[i];
			e->inUse = 1;
			return e;
		}
	}
	return NULL;
}

Building* building_setup(Building* e, int type)
{
	if (e)
	{
		//variables
		e->position.x = 0;
		e->position.y = 0;
		e->scale.x = 25;
		e->scale.y = 25;
		e->sprite = gf2d_sprite_load_all("images/square.png", 4, 4, 1);
		e->size.x = 4 * e->scale.x;
		e->size.y = 4 * e->scale.y;
		e->bounding_box.h = e->size.x;
		e->bounding_box.w = e->size.y;
		e->bounding_box.x = e->position.x;
		e->bounding_box.y = e->position.y;
		
		//function pointers
		e->update = building_update;
		e->free = building_free;

		//color
		if (type == 1) //Under construction
		{
			Vector4D temp_vec = { 128, 128, 128, 255 };
			e->color = temp_vec;
		}
		else if (type == 2) //Apartment
		{
			Vector4D temp_vec = { 255, 255, 255, 255 };
			e->color = temp_vec;
		}
		else if (type == 3) //Elevator
		{
			Vector4D temp_vec = { 255, 195, 0, 255 };
			e->color = temp_vec;
		}
		else
		{
			Vector4D temp_vec = { 0, 0, 0, 0 };
			e->color = temp_vec;
			slog("EMPTY BUILDING");
		}
	}

	//slog("ent setup");
	return e;
}

void update_buildings()
{
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (buildingList[i].inUse)
		{
			Building *e = &buildingList[i];
			(*e->update)(e);
		}
	}
}

void building_free(Building** e)
{
	(*e)->inUse = 0;
	*e = 0;
}

void building_update(Building* e)
{
	//if entity is out of screen bounds, free it
	if (e->position.x > 1200 || e->position.x < 0 || e->position.y > 720 || e->position.y < 0)
	{
		building_free(&e);
		return;
	}

	e->bounding_box.h = e->size.x;
	e->bounding_box.w = e->size.y;
	e->bounding_box.x = e->position.x;
	e->bounding_box.y = e->position.y;

	gf2d_sprite_draw(e->sprite, e->position, &e->scale, NULL, NULL, NULL, &e->color, 0);

	//bounding box DEBUG
	Vector4D col = { 255, 0, 255, 255 };
	gf2d_draw_rect(e->bounding_box, col);
}

void building_set_position(Building* e, int x, int y)
{
	if (e)
	{
		e->position.x = x;
		e->position.y = y;
	}
}

int building_count()
{
	int count = 0;
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (buildingList[i].inUse)
		{
			count++;
		}
	}
	return count;
}