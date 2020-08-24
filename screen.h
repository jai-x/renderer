#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct {
	int w; int h;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
} screen;

screen* screen_alloc(int w, int h, const char* title);
void screen_free(screen* s);
bool screen_is_alive(screen* s);
void screen_clear(screen* s);
void screen_present(screen* s);
void screen_set_color(screen* s, int r, int g, int b, int a);
void screen_set_point(screen* s, int x, int y);
