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

int level_array[MAX_LEVEL_WIDTH];
int influence = 100;
int happiness_avg = 0;
float dtime = 0;
int in_emergency = 0;

Mix_Music* music; 

int main(int argc, char * argv[])
{
	//ui element
	ui_element *influence_ui;
	ui_element *happiness_ui;
	
	//my variables
	Entity* e;
	Building* b;
	Building* mouse_target = NULL;

    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
	SDL_Event this_event;
    Sprite *sprite;

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
	//sprite = gf2d_sprite_load_image("images/falowiec.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	clearEntList();
	clearBuildingList();
	
	happiness_ui = new_ui_element(0);
	influence_ui = new_ui_element(1);

	load_song();
	read_level_file();
	generate_level();
	play_song();

    /*main game loop*/
    while(!done)
    {
		dtime = delta_time();

        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        /*update things here*/
        mf+=0.1f;
        if(mf >= 16.0)mf = 0;

		if(SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (int i = 0; i < MAX_BUILDINGS; i++)
			{
				b = point_to_building(mx, my, &buildingList[i]);
				if (b)
				{
					mouse_target = b;
				}
			}
		}

		if (keys[SDL_SCANCODE_C])
		{
			mouse_target = NULL;
			//slog("\nmouse_target set to NULL");
		}
		
		if (keys[SDL_SCANCODE_E])
		{
			mouse_target->buildingType = EMERGENCY;
			in_emergency = 1;
		}

		if (keys[SDL_SCANCODE_B])
		{
			if (mouse_target)
			{
				if (mouse_target->buildingType == EMERGENCY)
				{
					resolve_emergency(mouse_target);
					mouse_target == NULL;
				}
				else
				{
					construction_to_apartment(mouse_target);
					mouse_target == NULL;
				}
			}
		}

		update_happiness(&happiness_avg);
		update_influence(&influence);
		building_emergency();
		spawn_new_residents();

        gf2d_graphics_clear_screen(); // clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
		
		//create entity on space
		if(keys[SDL_SCANCODE_SPACE])
		{
			e = entity_new();
			e = entity_setup_character(e);
			entity_set_position(e, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
		}
		 
        //backgrounds drawn first
        gf2d_sprite_draw_image(sprite,vector2d(0,0));
	
		update_buildings();
		update_entities();

        //UI elements last
		happiness_ui->update(happiness_ui, happiness_avg);
		influence_ui->update(influence_ui, influence);

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
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //printf("\n Rendering at %f FPS",gf2d_graphics_get_frames_per_second()); // print FPS to console
		
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
