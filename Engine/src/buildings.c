#include <SDL.h>
#include <time.h>  
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "def.h"
#include "buildings.h"

extern Building buildingList[MAX_BUILDINGS];
extern int influence;
extern int happiness_avg;
extern float dtime;
extern int in_emergency;
int random;
Sprite* store1;
Sprite* store2;

void clearBuildingList()
{
	srand(time(NULL));
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
	int random = rand() % 3;

	if (b)
	{
		//variables
		b->position.x = 0;
		b->position.y = 0;

		if (type == 6)
		{
			if (random == 0)
			{
				b->sprite = gf2d_sprite_load_all("images/circleShape.png", 100, 100, 1);
				b->scale.x = 1;
				b->scale.y = 1;
				b->size.x = 100 * b->scale.x;
				b->size.y = 100 * b->scale.y;
			}
			else if (random == 1)
			{
				b->sprite = gf2d_sprite_load_all("images/diagonalPattern.png", 100, 100, 1);
				b->scale.x = 1;
				b->scale.y = 1;
				b->size.x = 100 * b->scale.x;
				b->size.y = 100 * b->scale.y;
			}
			else if (random == 2)
			{
				b->sprite = gf2d_sprite_load_all("images/crossPattern.png", 100, 100, 1);
				b->scale.x = 1;
				b->scale.y = 1;
				b->size.x = 100 * b->scale.x;
				b->size.y = 100 * b->scale.y;
			}
			else
			{
				slog("Store loading error");
			}
		}
		else
		{
			b->sprite = gf2d_sprite_load_all("images/square.png", 4, 4, 1);
			b->scale.x = 25;
			b->scale.y = 25;
			b->size.x = 4 * b->scale.x;
			b->size.y = 4 * b->scale.y;
		}

		b->bounding_box.h = b->size.x;
		b->bounding_box.w = b->size.y;
		b->bounding_box.x = b->position.x;
		b->bounding_box.y = b->position.y;
		b->time_to_resolve = RESOLVE_TIME;
		b->time_to_destroy = DESTROY_TIME;
		b->occupied = 0;

		//function pointers
		b->update_b = building_update;
		b->free_b = building_free;
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
			(*b->update_b)(b);
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
	//if building is out of screen bounds, free it
	if (b->position.x > 1200 || b->position.x < 0 || b->position.y > 720 || b->position.y < 0)
	{
		building_free(&b);
		return;
	}

	b->bounding_box.h = b->size.x;
	b->bounding_box.w = b->size.y;
	b->bounding_box.x = b->position.x;
	b->bounding_box.y = b->position.y;

	//check if still in emergency
	if (b->buildingType == EMERGENCY)
	{
		b->time_to_destroy -= dtime;
		if (b->time_to_destroy <= 0)
		{
			b->buildingType = CONSTRUCTION;
			b->time_to_destroy = DESTROY_TIME;
			b->time_to_resolve = RESOLVE_TIME;
			in_emergency = 0;
		}
	}

	//check is still resolving
	if (b->buildingType == RESOLVING)
	{
		b->time_to_resolve -= dtime;
		if (b->time_to_resolve <= 0)
		{
			b->buildingType = APARTMENT;
			b->time_to_resolve = RESOLVE_TIME;
			b->time_to_destroy = DESTROY_TIME;
		}
	}
	//color
	if (b->buildingType == CONSTRUCTION)
	{
		Vector4D temp_vec = { 128, 128, 128, 255 };
		b->color = temp_vec;
	}
	else if (b->buildingType == APARTMENT) 
	{
		Vector4D temp_vec = { 255, 255, 255, 255 };
		b->color = temp_vec;
	}
	else if (b->buildingType == ELEVATOR)
	{
		Vector4D temp_vec = { 255, 220, 0, 255 };
		b->color = temp_vec;
	}
	else if (b->buildingType == EMERGENCY)
	{
		Vector4D temp_vec = { 255, 0, 0, 255 };
		b->color = temp_vec;
	}
	else if (b->buildingType == RESOLVING)
	{
		Vector4D temp_vec = { 255, 140, 0, 255 };
		b->color = temp_vec;
	}
	else if (b->buildingType == WORK)
	{
		Vector4D temp_vec = { 0, 140, 0, 255 };
		b->color = temp_vec;
	}
	else
	{
		Vector4D temp_vec = { 0, 0, 0, 0 };
		b->color = temp_vec;
	}

	gf2d_sprite_draw(b->sprite, b->position, &b->scale, NULL, NULL, NULL, &b->color, 0);

	//bounding box DEBUG
	#ifdef DEBUG 
	Vector4D col = { 255, 0, 255, 255 };
	gf2d_draw_rect(b->bounding_box, col);
	#endif

}

void building_set_type(Building* b, int type)
{
	b->buildingType = type;
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

int building_count_ofType(BuildingType type)
{
	int count = 0;
	for (int i = 0; i < MAX_BUILDINGS; i++)
	{
		if (buildingList[i].inUse && buildingList[i].buildingType == type)
		{
			count++;
		}
	}
	return count;
}

void construction_to_apartment(Building* b)
{
	if (b)
	{
		if (b->buildingType == CONSTRUCTION && influence >= 100)
		{
			b->buildingType = APARTMENT;
			influence = influence - 100;
		}
	}
}

void resolve_emergency(Building* b)
{
	if (b)
	{
		if (b->buildingType == EMERGENCY)
		{
			b->buildingType = RESOLVING;
			in_emergency = 0;
		}
	}
}