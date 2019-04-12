#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct { float x, y, z; } vec3f;

void
vec3f_inspect(vec3f v) {
	printf("x: %f, y: %f, z: %f\n", v.x, v.y, v.x);
}

typedef struct { int x, y, z; } vec3i;

void
vec3i_inspect(vec3i v)
{
	printf("x: %i, y: %i, z: %i\n", v.x, v.y, v.x);
}

typedef struct {
	size_t v_size;
	vec3f* v;
	size_t f_size;
	vec3i* f;
} model;

vec3f
parse_vertex(const char* line)
{
	vec3f out;
	char* next;

	// Start after the leading 'v' and store pointer to beginning of next number
	out.x = strtof(&line[2], &next);
	out.y = strtof(next, &next);
	out.z = strtof(next, NULL);

	return out;
}

vec3i
parse_face(const char *line)
{
	vec3i out;
	char* next;

	// Start after the leading 'f' and store pointer to beginning of next number
	out.x = strtol(&line[2], &next, 10);
	out.y = strtol(next, &next, 10);
	out.z = strtol(next, NULL, 10);

	return out;
}

model
model_new(const char* filename)
{
	model out;
	out.v_size = 0;
	out.f_size = 0;

	char line[256];
	FILE* obj = fopen(filename, "r");

	// Counts
	while (fgets(line, sizeof(line), obj)) {
		switch (line[0]) {
		case 'v':
			out.v_size++;
			break;
		case 'f':
			out.f_size++;
			break;
		}
	}
	rewind(obj); // Move file pointer back to start
	// printf("Vertices: %zu, Faces: %zu\n", out.v_size, out.f_size);

	// Allocate based on count
	out.v = malloc(out.v_size * sizeof(vec3f));
	out.f = malloc(out.f_size * sizeof(vec3i));

	// Index to assign newly parsed values
	int v_current = 0;
	int f_current = 0;

	// Parsing
	while (fgets(line, sizeof(line), obj)) {
		switch (line[0]) {
		case 'v': {
			out.v[v_current++] = parse_vertex(line);
			break;
		}
		case 'f':
			out.f[f_current++] = parse_face(line);
			break;
		}
	}

	fclose(obj);
	return out;
}

void
model_free(model m)
{
	free(m.v);
	free(m.f);
}

typedef struct { uint8_t r, g, b; } color;
typedef struct { color** raw; int w, h; } buffer;

buffer
buffer_new(int w, int h)
{
	buffer out;
	out.w = w;
	out.h = h;
	out.raw = malloc(w * sizeof(color*));
	for (int i = 0; i < w; i++) {
		out.raw[i] = calloc(h, sizeof(color));
	}
	return out;
}

void
buffer_free(buffer b)
{
	for (int i = 0; i < b.w; i++) {
		free(b.raw[i]);
	}
	free(b.raw);
}

void
buffer_write(buffer b, const char* filename)
{
	FILE* out = fopen(filename, "w+");
	fprintf(out, "P3\n%d %d\n255", b.w, b.h);
	for (int y = 0; y < b.h; y++) {
		fprintf(out, "\n");
		for (int x = 0; x < b.w; x++) {
			color p = b.raw[x][y];
			fprintf(out, "%d %d %d ", p.r, p.g, p.b);
		}
	}
	fclose(out);
}

void
buffer_put_color(buffer b, int x, int y, color p)
{
	if (x >= b.w || y >= b.h) {
		fprintf(stderr,
			    "Buffer OOB access: x: %d, y: %d, w: %d, h: %d",
			    x, y, b.w, b.h);
		return;
	}

	b.raw[x][y] = p;
}

void
int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Some sort of attempt at implementing Bresenham's line algorithm.
// Can't quite figure out how to do this without floats so it's kinda slow.
// Also don't want to aimlessly copy a solution so stuck with this for now.
void
buffer_line(buffer b, int x1, int y1, int x2, int y2, color c)
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

int
normalise(float coord, float coord_max, int window_max)
{
	return (window_max * (coord + coord_max) / (coord_max * 2)) / 2;
	// printf("coord: %f, coord_max: %f, window_max: %d res: %d\n", coord, coord_max, window_max, res);
	// return res;
}

int
main(void)
{
	buffer img = buffer_new(1000, 1000);

	color blue   = {  0,   0, 255};
	color green  = {  0, 255,   0};
	color red    = {255,   0,   0};
	color yellow = {255, 255,   0};
	color white  = {255, 255, 255};

	buffer_line(img, 499, 0, 499, 999, blue);
	buffer_line(img, 0, 499, 999, 499, blue);

	buffer_line(img, 0, 0, 999, 499, red);
	buffer_line(img, 999, 599, 0, 99, green);

	buffer_line(img, 0, 499, 999, 0, yellow);
	buffer_line(img, 999, 99, 0, 599, white);

	buffer_line(img, 0, 0, 499, 999, red);
	buffer_line(img, 599, 999, 99, 0, green);

	buffer_line(img, 499, 999, 999, 0, yellow);
	buffer_line(img, 899, 0, 399, 999, white);

	buffer_write(img, "output.ppm");
	buffer_free(img);

	return EXIT_SUCCESS;
}
