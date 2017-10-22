#include <SDL.h>
#include <stdio.h>
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
extern level_array[MAX_LEVEL_WIDTH];
extern int happiness_avg;
extern float dtime;
extern int in_emergency;

FILE *myFile;
Building* b;
int influence_timer = INFLUENCE_TICK;
int event_timer = EVENT_TICK;
int event_change = EVENT_CHANCE;

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
	myFile = fopen("resources/level.txt", "r");

	if (myFile == NULL)
	{
		slog("Error Reading File");
	}

	for (int i = 0; i <= MAX_LEVEL_WIDTH; i++)
	{
		fscanf(myFile, "%d,", &level_array[i]);
	}

	fclose(myFile);
}

void generate_level()
{	
	for (int j = 0; j <= MAX_LEVEL_WIDTH; j++)
	{
		int xPos = WINDOW_WIDTH;
		int yPos = WINDOW_HEIGHT;

		if (level_array[j] != 0)
		{
			b = building_new();
			b = building_setup(b, level_array[j]);
			yPos = yPos - (yPos / 4);
			xPos = 10 + (j * 100);
			building_set_position(b, xPos, yPos);
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
					b->buildingType = EMERGENCY;
					event_timer = EVENT_TICK;
					in_emergency = 1;
					return;
				}
			}
		}
	}
}