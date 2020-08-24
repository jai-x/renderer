#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "screen.h"

screen*
screen_alloc(int w, int h, const char* title)
{
	screen* s = malloc(sizeof(screen));

	s->w = w;
	s->h = h;

	SDL_Init(SDL_INIT_VIDEO);

	s->window = SDL_CreateWindow(
		title,                  // window title
		SDL_WINDOWPOS_CENTERED, // window x position
		SDL_WINDOWPOS_CENTERED, // window y position
		w,                      // window width
		h,                      // window height
		SDL_WINDOW_SHOWN        // window flags
	);

	s->renderer = SDL_CreateRenderer(
		s->window,
		-1,
		SDL_RENDERER_SOFTWARE
	);

	return s;
}

void
screen_free(screen* s)
{
	SDL_DestroyRenderer(s->renderer);
	SDL_DestroyWindow(s->window);
	SDL_Quit();

	s->renderer = NULL;
	s->window = NULL;

	free(s);
}

bool
screen_is_alive(screen* s)
{
	if (SDL_PollEvent(&s->event)) {
		if (s->event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void
screen_clear(screen* s)
{
	SDL_RenderClear(s->renderer);
}

void
screen_present(screen* s)
{
	SDL_RenderPresent(s->renderer);
}

void
screen_set_color(screen* s, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(s->renderer, r, g, b, a);
}

void
screen_set_point(screen* s, int x, int y)
{
	SDL_RenderDrawPoint(s->renderer, x, y);
}
