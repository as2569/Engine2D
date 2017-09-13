#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "SDL.h"
#include "entity.h"

Entity entList[MAX_ENTITIES];

void clearEntList()
{
	memset(entList, 0, 50 * sizeof(Entity));
}

Entity* entity_new()
{
	SDL_Rect r = { 600, 360, 64, 64 };
	Vector4D col = { 255, 0, 0, 200 };

	for (int i = 0; i <= MAX_ENTITIES; i++)
	{
		if (!entList[i].inUse)
		{
			Entity *ent = &entList[i];
			entList[i].inUse = 1;
			entList[i].position.x = 0;
			entList[i].position.y = 0;
			entList[i].rect = r;
			entList[i].color = col;
			return ent;
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
	e->rect.y -= 0.01f;
	gf2d_draw_rect(e->rect, e->color);
}

void entity_set_position(Entity* e, int x, int y)
{
	e->position.x = x;
	e->position.y = y;
}

