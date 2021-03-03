#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <float.h>

#include "screen.h"
#include "stb/stb_image_write.h"

screen*
screen_alloc(int w, int h)
{
	screen* s = malloc(sizeof(screen));

	// screen size
	s->w = w;
	s->h = h;

	// set intial color
	s->r = 0;
	s->g = 0;
	s->b = 0;
	s->a = 255;

	// allocate buffer
	s->buffer = calloc((size_t)(s->w * s->h), sizeof(pixel));

	// allocate z buffer
	s->z_buffer = calloc((size_t)(s->w * s->h), sizeof(float));

	screen_clear(s);

	return s;
}

void
screen_write_tga(screen* s, const char* filename)
{
	stbi_flip_vertically_on_write(1);
	stbi_write_tga(filename, s->w, s->h, sizeof(pixel), s->buffer);
}

void
screen_write_png(screen* s, const char* filename)
{
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filename, s->w, s->h, sizeof(pixel), s->buffer, (s->w * (int)sizeof(pixel)));
}

void
screen_free(screen* s)
{
	free(s->buffer);
	free(s->z_buffer);

	s->buffer = NULL;
	s->z_buffer = NULL;

	free(s);
}

void
screen_clear(screen* s)
{
	// set buffer to current color
	for (int i = 0; i < (s->w * s->h); i++) {
		s->buffer[i][0] = s->r;
		s->buffer[i][1] = s->g;
		s->buffer[i][2] = s->b;
		s->buffer[i][3] = s->a;
	}

	// set z_buffer to be smallest floating point value
	for (int i = 0; i < (s->w * s->h); i++) {
		s->z_buffer[i] = -FLT_MAX;
	}
}

void
screen_set_color(screen* s, int r, int g, int b, int a)
{
	s->r = (unsigned char) r;
	s->g = (unsigned char) g;
	s->b = (unsigned char) b;
	s->a = (unsigned char) a;
}

static inline int
idx(screen* s, int x, int y)
{
	// OOB check
	assert((x >= 0) && (y >= 0) && (x <= s->w) && (y <= s->h));

	return (x + (y * s->w));
}

void
screen_set_point(screen* s, int x, int y)
{
	int pxl = idx(s, x, y);

	s->buffer[pxl][0] = s->r;
	s->buffer[pxl][1] = s->g;
	s->buffer[pxl][2] = s->b;
	s->buffer[pxl][3] = s->a;
}

float
screen_get_z(screen* s, int x, int y)
{
	return s->z_buffer[idx(s, x, y)];
}

void
screen_set_z(screen* s, int x, int y, float z)
{
	s->z_buffer[idx(s, x, y)] = z;
}
