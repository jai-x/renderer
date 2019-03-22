#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef struct { uint8_t r, g, b; } color;
typedef struct { color** raw; int w, h; } buffer;

buffer
new_buffer(int w, int h)
{
	buffer out;
	out.w = w;
	out.h = h;
	out.raw = malloc(w * sizeof (color*));
	for (int i = 0; i < w; i++) {
		out.raw[i] = calloc(h, sizeof (color));
	}
	return out;
}

void
free_buffer(buffer b)
{
	for (int i = 0; i < b.w; i++) {
		free(b.raw[i]);
	}
	free(b.raw);
}

void
write_buffer(buffer b)
{
	FILE* out = fopen("output.ppm", "w+");
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
put_color(buffer b, int x, int y, color p)
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

// Some sort of failed attempt at implementing Bresenham's line algorithm
void
line(buffer b, int x1, int y1, int x2, int y2, color p)
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
		transpose ? put_color(b, y, x, p) : put_color(b, x, y, p);
	};
}

int
main(void)
{
	buffer buf = new_buffer(1000, 1000);

	color red = {255, 0, 0};
	color green = {0, 255, 0};
	color yellow = {255, 255, 0};
	color white = {255, 255, 255};
	color blue = {0, 0, 255};

	line(buf, 499, 0, 499, 999, blue);
	line(buf, 0, 499, 999, 499, blue);

	line(buf, 0, 0, 999, 499, red);
	line(buf, 999, 599, 0, 99, green);

	line(buf, 0, 499, 999, 0, yellow);
	line(buf, 999, 99, 0, 599, white);

	line(buf, 0, 0, 499, 999, red);
	line(buf, 599, 999, 99, 0, green);

	line(buf, 499, 999, 999, 0, yellow);
	line(buf, 899, 0, 399, 999, white);

	write_buffer(buf);
	free_buffer(buf);
	return EXIT_SUCCESS;
}
