#include <stdbool.h>
#include <math.h>
#include <string.h>
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

	// create software renderer that outputs directly to the window surface
	s->renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(s->window));

	// get real height and width from renderer size
	SDL_GetRendererOutputSize(s->renderer, &s->w, &s->h);

	// allocate z buffer based on screen size
	s->z_buffer = calloc((s->w * s->h), sizeof(float));

	return s;
}

void
screen_free(screen* s)
{
	SDL_DestroyRenderer(s->renderer);
	SDL_DestroyWindow(s->window);
	SDL_Quit();

	free(s->z_buffer);

	s->z_buffer = NULL;
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

	// set z_buffer to be smallest floating point value
	for (int i = 0; i < (s->w * s->h); i++) {
		s->z_buffer[i] = -FLT_MAX;
	}

	s->ticks = SDL_GetTicks();
}

void
screen_present(screen* s)
{
	// Since the renderer was created with `SDL_CreateSoftwareRenderer` we
	// call `SDL_UpdateWindowSurface()` instead of `SDL_RenderPresent` as the
	// SDL_Renderer in software mode has already made the changes to the window
	// surface, so we just need to window to present the suface changes to the
	// framebuffer
	SDL_UpdateWindowSurface(s->window);

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

float
screen_get_z(screen* s, int x, int y)
{
	// OOB check
	assert((x >= 0) && (y >= 0) && (x <= s->w) && (y <= s->h));

	return s->z_buffer[x + (y * s->w)];
}

void
screen_set_z(screen* s, int x, int y, float z)
{
	// OOB check
	assert((x >= 0) && (y >= 0) && (x <= s->w) && (y <= s->h));

	s->z_buffer[x + (y * s->w)] = z;
}
