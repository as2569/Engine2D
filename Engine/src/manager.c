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
extern level_array[16];

FILE *myFile;
Building* b;

void update_content(int *happiness_average)
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
	(*happiness_average) = satisfaction;
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