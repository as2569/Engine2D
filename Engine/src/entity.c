#include <SDL.h>
#include <stdio.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "buildings.h"
#include "entity.h"
#include "def.h"

extern Entity entList[MAX_ENTITIES];
extern float dtime;

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
		e->velocity.x = 0;
		e->velocity.y = 0;
		e->happiness = 25;
		e->internal_time = 500;
		e->state_time = STATE_TIME;

		//function pointers
		e->update_e = entity_update;
		e->free_e = entity_free;
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
			Entity *e = &entList[i];
			(*e->update_e)(e);
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
	////if entity is out of screen bounds, free it
	//if (e->position.x > 1200 || e->position.x < 0 || e->position.y > 720 || e->position.y < 0)
	//{
	//	entity_free(&e);
	//	return;
	//}

	//update time and happiness
	e->internal_time -= dtime;
	e->state_time -= dtime;
	if (e->internal_time <= 0)
	{
		e->internal_time = 500;
		e->happiness -= 1;
	}

	set_state(e);
	printf("\nstate %i, time %i", e->state, e->state_time);
	//vec_to_vec(e->position, e->destination);


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

int entity_count()
{
	int count = 0;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (entList[i].inUse)
		{
			count++;
		}
	}
	return count;
}

Entity* entity_set_destination(Entity* e, float x, float y)
{
	e->destination.x = x;
	e->destination.y = y;

	return e;
}

Entity* entity_set_home(Entity* e, float x, float y)
{
	e->home.x = x;
	e->home.y = y;
	
	return e;
}

Entity* entity_set_work(Entity* e, float x, float y)
{
	e->work.x = x;
	e->work.y = y;

	return e;
}

int vec_to_vec(Vector2D this_vec, Vector2D other_vec)
{
	double distance;
	distance = sqrt((this_vec.x - other_vec.x) * (this_vec.x - other_vec.x)
		+ (this_vec.y - other_vec.y) * (this_vec.y - other_vec.y));

	printf("(%f,%f) [%f,%f] dis %f\n", this_vec.x, this_vec.y, other_vec.x, other_vec.y, distance);
	if (distance <= 20)
	{
		return 1;
		printf("vec collision");
	}
	return 0;
}

void set_state(Entity* e)
{
	//if going to work set vel.x to -1
	if(e->state == GOINGWORK)
	{
		if (vec_to_vec(e->position, e->destination) == 1)
		{
			e->state = ATWORK;
			e->state_time = STATE_TIME;
			e->velocity.x = 0;
		}
		else
		{
			e->velocity.x = -1;
		}
	}

	//if going home set vel.x to 1
	if (e->state == GOINGHOME)
	{
		if (vec_to_vec(e->position, e->destination) == 1)
		{
			e->state = ATHOME;
			e->state_time = STATE_TIME;
			e->velocity.x = 0;
		}
		else
		{
			e->velocity.x = 1;
		}
	}

	//if at home and time is out switch destination to work
	if (e->state == ATHOME && e->state_time <= 0)
	{
		e->state_time = STATE_TIME;
		e->destination = e->work;
		e->state = GOINGWORK;
	}
	
	if (e->state == ATWORK && e->state_time <= 0)
	{
		e->state_time = STATE_TIME;
		e->destination = e->home;
		e->state = GOINGHOME;
	}
}
