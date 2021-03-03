#pragma once

typedef unsigned char pixel[4];

typedef struct {
	int w;
	int h;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	pixel* buffer;

	float* z_buffer;
} screen;

screen* screen_alloc(int w, int h);
void screen_write_tga(screen* s, const char* filename);
void screen_write_png(screen* s, const char* filename);
void screen_free(screen* s);
void screen_clear(screen* s);
void screen_set_color(screen* s, int r, int g, int b, int a);
void screen_set_point(screen* s, int x, int y);
float screen_get_z(screen* s, int x, int y);
void screen_set_z(screen* s, int x, int y, float z);
