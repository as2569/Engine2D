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
	Sprite* new_sprite = gf2d_sprite_load_all("images/testSprites.png", 32, 32, 1);

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (!entList[i].inUse)
		{
			Entity *e = &entList[i];
			entList[i].inUse = 1;
			entList[i].position.x = 0;
			entList[i].position.y = 0;
			entList[i].scale.x = 2;
			entList[i].scale.y = 2;
			entList[i].sprite = new_sprite;
			e->velocity.x = gf2d_crandom();
			e->velocity.y = gf2d_crandom();
			return e;
		}
	}
	return NULL;
}

void update_entities()
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (entList[i].inUse)
		{
			entList[i].update = entity_update;
			Entity *e = &entList[i];
			(*e->update)(e);
		}
	}
}

//why is this double pointer?
void entity_free(Entity** e)
{
	(*e)->inUse = 0;
	*e = 0;
}

//void entity_free(Entity* e)
//{
//	e->inUse = 0;
//	e->sprite = NULL;
//	e = 0;
//}

void entity_update(Entity* e)
{
	//if entity is out of screen bounds, free it
	if (e->position.x > 1250 || e->position.x < -1250 || e->position.y > 750 || e->position.y < -750)
	{
		entity_free(&e);
		slog("entity out of bounds, deleted");
		return;
	}

	vector2d_add(e->position, e->position, e->velocity);
	gf2d_sprite_draw(e->sprite, e->position, &e->scale, NULL, NULL, NULL, NULL, 5);

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

