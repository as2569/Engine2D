#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <time.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "manager.h"
#include "buildings.h"
#include "entity.h"
#include "collisions.h"
#include "def.h"
#include "sound.h"
#include "user_interface.h"

Entity entList[MAX_ENTITIES];
Building buildingList[MAX_BUILDINGS];
ui_element* uiList[MAX_UI_ELEMENTS];

int level_array[MAX_LEVEL_HEIGHT][MAX_LEVEL_WIDTH];
int influence = 100;
int happiness_avg = 0;
float dtime = 0;
int in_emergency = 0;

Mix_Music* music; 

int main(int argc, char * argv[])
{	
	//my variables
	Entity* e;
	Building* b;
	Building* mouse_target = NULL;
	ui_element* ui = NULL;

    /*variable declarations*/
    int quit = 0;
	int in_main = 1;
    const Uint8 * keys;
	SDL_Event this_event;
    Sprite* sprite;
	Sprite* titleScreen;

	//mouse variables
    int mx,my;
    float mf = 0;
    Sprite *mouse;
	
    Vector4D mouseColor = {255,100,255,200};

    /*program initialization*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	titleScreen = gf2d_sprite_load_image("images/falowiec.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	clearEntList();
	clearBuildingList();
	init_ui();

	load_song();
	read_level_file();
	generate_level();
	play_song();

    /*main game loop*/
	while (!quit)
	{
		//slog("in main %i", in_main);
		if (SDL_PollEvent(&this_event))
		{
			if (this_event.type == SDL_QUIT)
			{
				quit = 1;
			}

			/*update things here*/
			dtime = delta_time();

			SDL_GetMouseState(&mx, &my);

			if (this_event.type == SDL_MOUSEBUTTONUP)
			{
				for (int i = 0; i < MAX_BUILDINGS; i++) //check if clicked on buildings
				{					
					if ((b = point_to_building(mx, my, &buildingList[i])) != NULL)
					{
						break;
					}
				}

				if (b != NULL)
				{
					mouse_target = b;
				}

				for (int j = 0; j < MAX_UI_ELEMENTS; j++) //check if clicked on ui
				{
					ui = point_to_ui(mx, my, uiList[j]);
					if (ui)
					{
						if (ui->isClickable == 1 && ui->isActive == 1)
						{
							if (ui->ui_type == 0)
							{
								slog("passing %i ", in_main);
								ui->click(ui, &in_main);
								slog("current %i ", in_main);
							}
							else if (ui->ui_type == 4)
								ui->click(ui, mouse_target);
							else
								ui->click(ui);
						}
					}
				}
			}
		}

		if (in_main == 0)
		{
			update_happiness(&happiness_avg);
			update_influence(&influence);
			building_emergency();
			spawn_new_residents();
		}

		gf2d_graphics_clear_screen(); // clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
	
		if (in_main)
		{
			gf2d_sprite_draw_image(titleScreen, vector2d(0, 0)); //backgrounds drawn first
		}
		else
		{
			gf2d_sprite_draw_image(sprite, vector2d(0, 0)); //backgrounds drawn first
			update_buildings();
			update_entities();			
		}

		update_ui(); //UI elements last

		gf2d_sprite_draw(
			mouse,
			vector2d(mx, my),
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			5);

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

	}

	slog("---==== END ====---");
}
/*eol@eof*/
