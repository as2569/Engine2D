#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"

Entity entList[MAX_ENTITIES];

void clearEntList()
{
	memset(entList, 0, MAX_ENTITIES * sizeof(Entity));
}

Entity* entity_new()
{
	Entity *e = NULL;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!entList[i].inUse)
		{
			Entity *e = &entList[i];
			e->inUse = 1;
			//slog("new ent");
			return e;
		}
	}
	return NULL;
}

Entity* entity_setup_character(Entity* e)
{
	if (e)
	{
		//variables
		e->position.x = 0;
		e->position.y = 0;
		e->scale.x = 2;
		e->scale.y = 2;
		e->sprite = gf2d_sprite_load_all("images/testSprites.png", 32, 32, 1);
		e->size.x = 32 * e->scale.x;
		e->size.y = 32 * e->scale.y;
		e->bounding_box.h = e->size.x;
		e->bounding_box.w = e->size.y;
		e->bounding_box.x = e->position.x;
		e->bounding_box.y = e->position.y;
		e->velocity.x = gf2d_crandom();
		e->velocity.y = gf2d_crandom();

		//function pointers
		e->update = entity_update;
		e->free = entity_free;
	}
	//slog("ent setup");
	return e;
}

void update_entities()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (entList[i].inUse)
		{
			//entList[i].update = entity_update; //no longer setup function pointer in update
			Entity *e = &entList[i];
			(*e->update)(e);
		}
	}
}

void entity_free(Entity** e)
{
	(*e)->inUse = 0;
	*e = 0;
}

void entity_update(Entity* e)
{
	//if entity is out of screen bounds, free it
	if (e->position.x > 1200 || e->position.x < 0 || e->position.y > 720 || e->position.y < 0)
	{
		entity_free(&e);
		//slog("entity out of bounds, deleted");
		return;
	}

	vector2d_add(e->position, e->position, e->velocity);
	e->bounding_box.h = e->size.x;
	e->bounding_box.w = e->size.y / 2;
	e->bounding_box.x = e->position.x + e->bounding_box.w / 2;
	e->bounding_box.y = e->position.y;

	gf2d_sprite_draw(e->sprite, e->position, &e->scale, NULL, NULL, NULL, NULL, 5);

	//bounding box DEBUG
	Vector4D col = { 255, 0, 255, 255 };
	gf2d_draw_rect(e->bounding_box, col);
}

void entity_set_position(Entity* e, int x, int y)
{
	if (e)
	{
		e->position.x = x;
		e->position.y = y;
	}
}

void entity_set_velocity(Entity* e, float x, float y)
{
	if (e)
	{
		e->velocity.x = x;
		e->velocity.y = y;
	}
}

