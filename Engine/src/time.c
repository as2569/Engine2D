#include "time.h"
#include "def.h"

int old_time;
int current_time;
float ftime;

float delta_time()
{
	old_time = current_time;
	current_time = SDL_GetTicks();
	ftime = (current_time - old_time) / 1000.0f;
	return ftime;
}