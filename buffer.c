#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "buffer.h"
#include "vec3f.h"
#include "vec2i.h"

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
buffer_print(buffer* b)
{
	printf("w: %d, h: %d\n", b->w, b->h);
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

// Find barycentric coordinates of point `p` with respect to triangle
// represented by points `a`, `b`, and `c`
static vec3f
barycentric(vec2i a, vec2i b, vec2i c, vec2i p)
{
	vec3f out = {0, 0, 0};

	// x and y of vectors A->C, A->B, and A->P
	vec3f ux = {c.x - a.x, b.x - a.x, a.x - p.x};
	vec3f uy = {c.y - a.y, b.y - a.y, a.y - p.y};

	vec3f u = vec3f_cross(ux, uy);

	// check for degenerate triangle
	if (fabsf(u.z) < 1) {
		out.x = -1; // set coordinate outside triangle
		return out;
	}

	out.x = 1.0f - ((u.x + u.y) / u.z);
	out.y = u.y / u.z;
	out.z = u.x / u.z;
	return out;
}

void
buffer_triangle(buffer* b, vec2i v0, vec2i v1, vec2i v2, color c)
{
	// bounding box minimum coordinates
	vec2i box_min = {b->w - 1, b->h - 1};
	box_min = vec2i_min_values(box_min, v0);
	box_min = vec2i_min_values(box_min, v1);
	box_min = vec2i_min_values(box_min, v2);

	// bounding box maximum coordinates
	vec2i box_max = {0, 0};
	box_max = vec2i_max_values(box_max, v0);
	box_max = vec2i_max_values(box_max, v1);
	box_max = vec2i_max_values(box_max, v2);

	// point in buffer to check
	vec2i p;

	for (p.x = box_min.x; p.x < box_max.x; p.x++) {
		for (p.y = box_min.y; p.y < box_max.y; p.y++) {
			// find barycentric coordinates of point p with respect to the given triangle vertices
			vec3f bc = barycentric(v0, v1, v2, p);

			// coordinates less than zero fall outside the triangle and are skipped
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
				continue;
			}

			buffer_put_color(b, p.x, p.y, c);
		}
	}
}
