#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "def.h"
#include "buildings.h"

extern Building buildingList[MAX_BUILDINGS];
extern int influence;

void clearBuildingList()
{
	memset(buildingList, 0, MAX_BUILDINGS * sizeof(Building));
}

Building* building_new()
{
	Building *b = NULL;
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (!buildingList[i].inUse)
		{
			Building *b = &buildingList[i];
			b->inUse = 1;
			return b;
		}
	}
	return NULL;
}

Building* building_setup(Building* b, int type)
{
	if (b)
	{
		//variables
		b->position.x = 0;
		b->position.y = 0;
		b->scale.x = 25;
		b->scale.y = 25;
		b->sprite = gf2d_sprite_load_all("images/square.png", 4, 4, 1);
		b->size.x = 4 * b->scale.x;
		b->size.y = 4 * b->scale.y;
		b->bounding_box.h = b->size.x;
		b->bounding_box.w = b->size.y;
		b->bounding_box.x = b->position.x;
		b->bounding_box.y = b->position.y;

		//function pointers
		b->update = building_update;
		b->free = building_free;

		//color
		if (type == 1) //Under construction
		{
			b->buildingType = CONSTRUCTION;
		}
		else if (type == 2) //Apartment
		{
			b->buildingType = APARTMENT;
		}
		else if (type == 3) //Elevator
		{
			b->buildingType = ELEVATOR;
		}
		else
		{
			b->buildingType = EMPTY;
		}

	}

	//slog("ent setup");
	return b;
}

void update_buildings()
{
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (buildingList[i].inUse)
		{
			Building *b = &buildingList[i];
			(*b->update)(b);
		}
	}
}

void building_free(Building** b)
{
	(*b)->inUse = 0;
	*b = 0;
}

void building_update(Building* b)
{
	//if entity is out of screen bounds, free it
	if (b->position.x > 1200 || b->position.x < 0 || b->position.y > 720 || b->position.y < 0)
	{
		building_free(&b);
		return;
	}

	b->bounding_box.h = b->size.x;
	b->bounding_box.w = b->size.y;
	b->bounding_box.x = b->position.x;
	b->bounding_box.y = b->position.y;

	//color
	if (b->buildingType == 1) //Under construction
	{
		Vector4D temp_vec = { 128, 128, 128, 255 };
		b->color = temp_vec;
		b->buildingType = CONSTRUCTION;
	}
	else if (b->buildingType == 2) //Apartment
	{
		Vector4D temp_vec = { 255, 255, 255, 255 };
		b->color = temp_vec;
		b->buildingType = APARTMENT;
	}
	else if (b->buildingType == 3) //Elevator
	{
		Vector4D temp_vec = { 255, 195, 0, 255 };
		b->color = temp_vec;
		b->buildingType = ELEVATOR;
	}
	else
	{
		Vector4D temp_vec = { 0, 0, 0, 0 };
		b->color = temp_vec;
		b->buildingType = EMPTY;
		slog("EMPTY BUILDING");
	}

	gf2d_sprite_draw(b->sprite, b->position, &b->scale, NULL, NULL, NULL, &b->color, 0);

	//bounding box DEBUG
	Vector4D col = { 255, 0, 255, 255 };
	gf2d_draw_rect(b->bounding_box, col);

	

}

void building_set_position(Building* b, int x, int y)
{
	if (b)
	{
		b->position.x = x;
		b->position.y = y;
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

void construction_to_valid(Building* b)
{
	if (b)
	{
		if (b->buildingType == CONSTRUCTION && influence >= 100)
		{
			b->buildingType = APARTMENT;
			influence = influence - 100;
			slog("changed to apartment");
		}
		else
		{
			slog("building not construction type");
		}
	}
	else
	{
		slog("building in NULL");
	}
}