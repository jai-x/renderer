#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include <SDL2/SDL.h>

#include "screen.h"

static const uint32_t target_delta = 1000 / 30; // 30 fps

screen*
screen_alloc(int w, int h, const char* title)
{
	screen* s = malloc(sizeof(screen));

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

	SDL_GetRendererOutputSize(s->renderer, &s->w, &s->h);

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

screen_event
screen_check_events(screen* s)
{
	if (SDL_PollEvent(&s->event)) {
		if (s->event.type == SDL_QUIT) {
			return SCREEN_QUIT;
		}

		if (s->event.type == SDL_KEYDOWN) {
			// r key to redraw
			if (s->event.key.keysym.sym == SDLK_r) {
				return SCREEN_REDRAW;
			}

			// q key to quit
			if (s->event.key.keysym.sym == SDLK_q) {
				return SCREEN_QUIT;
			}
		}
	}

	return SCREEN_CONTINUE;
}

void
screen_clear(screen* s)
{
	SDL_RenderClear(s->renderer);

	s->ticks = SDL_GetTicks();
}

void
screen_present(screen* s)
{
	SDL_RenderPresent(s->renderer);

	// Add delay if rendering faster than the target delta
	uint32_t delta = SDL_GetTicks() - s->ticks;
	if (delta < target_delta) {
		SDL_Delay(delta - target_delta);
	}
}

void
screen_set_color(screen* s, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(s->renderer, r, g, b, a);
}

void
screen_set_point(screen* s, int x, int y)
{
	// OOB check
	assert((x >= 0) && (y >= 0) && (x <= s->w) && (y <= s->h));

	SDL_RenderDrawPoint(s->renderer, x, y);
}
