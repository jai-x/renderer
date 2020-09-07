#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

typedef struct {
	int w;
	int h;
	float* z_buffer;

	uint32_t ticks;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
} screen;

typedef enum { SCREEN_CONTINUE, SCREEN_QUIT, SCREEN_REDRAW } screen_event;

screen* screen_alloc(int w, int h, const char* title);
void screen_free(screen* s);
screen_event screen_check_events(screen* s);
void screen_clear(screen* s);
void screen_present(screen* s);
void screen_set_color(screen* s, int r, int g, int b, int a);
void screen_set_point(screen* s, int x, int y);
float screen_get_z(screen* s, int x, int y);
void screen_set_z(screen* s, int x, int y, float z);
