#include <SDL.h>
#include <stdio.h>
#include <physfs.h>
#include "simple_logger.h"
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "manager.h"
#include "buildings.h"
#include "entity.h"
#include "def.h"

extern Entity entList[MAX_ENTITIES];
extern Building buildingList[MAX_BUILDINGS];
extern level_array[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
extern int happiness_avg;
extern float dtime;
extern int in_emergency;
extern int in_main;
extern int lost;

FILE *myFile;
char* line[MAX_LEVEL_WIDTH];
Building* b;
Entity* e;
Building* workBuilding;
int i = 0;
int influence_timer = INFLUENCE_TICK;
int event_timer = EVENT_TICK;
int event_change = EVENT_CHANCE;
int temp;

void update_happiness(int *happiness)
{
	//move this to a separate file
	int temp = 0;
	int satisfaction = 0;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (entList[i].inUse == 1)
		{
			temp += entList[i].happiness;
		}
	}

	if (entity_count() != 0)
	{
		satisfaction = temp / entity_count();
	}
	(*happiness) = satisfaction;

	if (satisfaction == 1)
	{
		slog("Happiness average at 1, you lost");
		lost = 1;
	}
}

void update_influence(int* influence)
{
	influence_timer -= dtime;
	if (influence_timer <= 0)
	{
		if (in_emergency == 0)//if not in emergency
		{
			(*influence) = (*influence) + happiness_avg;
			influence_timer = INFLUENCE_TICK;
		}
		if (in_emergency == 1 && (*influence) > 0)
		{
			(*influence) = (*influence) - 25;
			influence_timer = INFLUENCE_TICK;
		}
	}
}

void read_level_file()
{
	PHYSFS_File* myfs_File = PHYSFS_openRead("level/level.txt");

	if (myfs_File == NULL)
	{
		slog("Error Reading File");
	}

	for (int j = 0; j < MAX_LEVEL_HEIGHT; j++)
	{
		if(PHYSFS_read(myfs_File, line, 20, 1) != NULL)
		{
			line[19] = '\0';
			//printf("%s\n", line);
			char *p = line;
			for (int i = 0; i < MAX_LEVEL_WIDTH; i++)
			{
				int temp = strtoll(p, &p, 10);
				level_array[j][i] = temp;
			}
		}
	}
	PHYSFS_close(myfs_File);
}

void generate_level()
{	
	for (int j = 0; j < MAX_LEVEL_HEIGHT; j++)
	{
		for (int k = 0; k < MAX_LEVEL_WIDTH; k++)
		{
			printf("%i", level_array[j][k]);

			int xPos = WINDOW_WIDTH;
			int yPos = WINDOW_HEIGHT;

			b = building_new();
			building_set_type(b, level_array[j][k]);
			b = building_setup(b, level_array[j][k]);

			yPos = (yPos - (yPos / 2)) + (j * 100);
			xPos = 10 + (k * 100);
			building_set_position(b, xPos, yPos);

		}
	}

	for (int k = 0; k <= MAX_BUILDINGS; k++)
	{
		b = &buildingList[k];
		if (b->buildingType == WORK)
		{
			workBuilding = b;
		}
	}
}

void building_emergency()
{
	float random;
	event_timer -= dtime;
	if (event_timer <= 0 && in_emergency == 0)
	{
		random = gf2d_random();
		if (random < EVENT_CHANCE)
		{
			for (int i = 0; i < MAX_BUILDINGS; i++)
			{
				b = &buildingList[i];
				if (b->buildingType == APARTMENT)
				{
					slog("building emergency");
					b->buildingType = EMERGENCY;
					event_timer = EVENT_TICK;
					in_emergency = 1;
					return;
				}
			}
		}
	}
}

void spawn_new_residents()
{
	if (entity_count() < building_count_ofType(APARTMENT))
	{
		for (int i = 0; i < MAX_BUILDINGS; i++)
		{
			b = &buildingList[i];
			if (b)
			{
				if (b->occupied == 0 && b->buildingType == APARTMENT)
				{
					float temp_x = b->position.x + 25;
					float temp_y = b->position.y + 25;
					e = entity_new();
					e = entity_setup_character(e);
					e->state = ATHOME;
					e = entity_set_home(e, b);
					e = entity_set_work(e, workBuilding);
					e = entity_set_destination(e, b);
					entity_set_position(e, temp_x, temp_y);
					b->occupied = 1;
				}
			}
		}

	}
}
