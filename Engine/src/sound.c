#include <SDL.h>
#include <SDL_mixer.h>
#include "simple_logger.h"
#include "sound.h"

extern Mix_Music *music;

void load_song()
{
	if ((music = Mix_LoadMUS("resources/njit.mp3")) == NULL)
	{
		slog("sdl_mixer error %s", Mix_GetError());
	}
}

void cleanup_song()
{
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

int play_song()
{
	//Open audio 
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512) == -1)
	{
		slog("audio fail");
	}

	//If there is no music playing
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(music, 1) == -1)
		{
			slog("Music playing");
		}
		return 1;
	}
}