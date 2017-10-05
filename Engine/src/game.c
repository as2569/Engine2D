#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "collisions.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

extern Entity entList[MAX_ENTITIES];

int main(int argc, char * argv[])
{
	//my variables
	Entity* e;
	int test;

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
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	clearEntList();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

        /*update things here*/
        mf+=0.1f;
        if(mf >= 16.0)mf = 0;
		
		if(SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (int i = 0; i < MAX_ENTITIES; i++)
			{
				e = point_to_entity(mx, my, &entList[i]);
				if(e)
				{
					e->free(&e);
				}
			}
		}

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

		update_entities();

        //UI elements last
		gf2d_sprite_draw(
			mouse,
			vector2d(mx, my),
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			(int)mf);

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //printf("\n Rendering at %f FPS",gf2d_graphics_get_frames_per_second()); // print FPS to console
		
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
