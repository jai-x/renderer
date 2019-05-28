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
	if (x >= b->w || y >= b->h) {
		fprintf(stderr,
			    "Buffer OOB access: x: %d, y: %d, w: %d, h: %d",
			    x, y, b->w, b->h);
		return;
	}

	b->raw[x][y] = p;
}


// Some sort of attempt at implementing Bresenham's line algorithm.
// Can't quite figure out how to do this without floats so it's kinda slow.
// Also don't want to aimlessly copy a solution so stuck with this for now.
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

	float dy = y2 - y1;
	float dx = x2 - x1;
	float m = dy/dx;

	for (int x = x1; x <= x2; x++) {
		int y = lroundf((x * m) + y1);
		transpose ? buffer_put_color(b, y, x, c) : buffer_put_color(b, x, y, c);
	};
}
