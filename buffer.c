#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "buffer.h"

static void
int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

buffer*
buffer_alloc(int w, int h)
{
	buffer* out = malloc(sizeof(buffer));

	out->w = w;
	out->h = h;
	out->raw = malloc(sizeof(color*) * w);

	for (int i = 0; i < w; i++) {
		out->raw[i] = calloc(h, sizeof(color));
	}
	return out;
}

void
buffer_free(buffer* b)
{
	for (int i = 0; i < b->w; i++) {
		free(b->raw[i]);
	}
	free(b->raw);
	free(b);
}

void
buffer_write(buffer* b, const char* filename)
{
	FILE* out = fopen(filename, "w+");
	fprintf(out, "P3\n%d %d\n255", b->w, b->h);

	for (int y = 0; y < b->h; y++) {
		fprintf(out, "\n");

		for (int x = 0; x < b->w; x++) {
			color p = b->raw[x][y];
			fprintf(out, "%d %d %d ", p.r, p.g, p.b);
		}
	}

	fclose(out);
}

void
buffer_put_color(buffer* b, int x, int y, color p)
{
	if (x >= b->w || x < 0 || y >= b->h || y < 0) {
		printf("Buffer OOB access: x: %d, y: %d, w: %d, h: %d\n", x, y, b->w, b->h);
		return;
	}

	b->raw[x][y] = p;
}


// Bresenham's line algorithm.
// I couldn't find a way to derive a non-buggy version myself so I just copied
// an implementation I knew was working from ssloy/tinyrenderer.
void
buffer_line(buffer* b, int x1, int y1, int x2, int y2, color c)
{
	bool transpose = false;

	if (abs(y2 - y1) > abs(x2 - x1)) {
		int_swap(&x1, &y1);
		int_swap(&x2, &y2);
		transpose = true;
	}

	if (x1 > x2) {
		int_swap(&x1, &x2);
		int_swap(&y1, &y2);
	}

	int dy = y2 - y1;
	int dx = x2 - x1;

	int derr = abs(dy) * 2;

	int y = y1;
	int err = 0;

	for (int x = x1; x <= x2; x++) {
		transpose ? buffer_put_color(b, y, x, c) : buffer_put_color(b, x, y, c);

		err += derr;

		if (err > dx) {
			y += (y2 > y1) ? 1 : -1;
			err -= dx * 2;
		}
	};
}

void
buffer_print(buffer* b)
{
	printf("w: %d, h: %d\n", b->w, b->h);
}
