#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "buildings.h"
#include "collisions.h"

Entity* point_to_entity(int x, int y, Entity* ent)
{
	if (ent->inUse)
	{
		if ((x >= ent->bounding_box.x)
			&& (x <= (ent->bounding_box.x + ent->bounding_box.w))
			&& (y >= ent->bounding_box.y)
			&& (y <= (ent->bounding_box.y + ent->bounding_box.h)))
		{
			//ent is an entity in use
			return ent;
		}
		else
		{
			//ent is not in use
			return NULL;
		}
	}
	else
	{
		//ent is not valid
		return NULL;
	}
}

Building* point_to_building(int x, int y, Building* bld)
{
		if ((x >= bld->bounding_box.x)
			&& (x <= (bld->bounding_box.x + bld->bounding_box.w))
			&& (y >= bld->bounding_box.y)
			&& (y <= (bld->bounding_box.y + bld->bounding_box.h)))
		{
			return bld;
		}
		else
		{
			return NULL;
		}
}