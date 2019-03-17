#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct pixel { uint8_t r, g, b; };
struct buffer { struct pixel** raw; size_t w, h; };

struct buffer
new_buffer(size_t w, size_t h)
{
	struct buffer out;
	out.w = w;
	out.h = h;
	out.raw = malloc(w * sizeof (struct pixel*));
	for (size_t i = 0; i < w; i++) {
		out.raw[i] = calloc(h, sizeof (struct pixel));
	}
	return out;
}

void
put_pixel(struct buffer b, size_t x, size_t y, struct pixel p)
{
	if (x >= b.w || y >= b.h) {
		fprintf(stderr,
			    "Buffer OOB access: x: %zu, y: %zu, w: %zu, h: %zu",
			    x, y, b.w, b.h);
		return;
	}

	b.raw[x][y] = p;
}

void
write_buffer(struct buffer b)
{
	FILE* out = fopen("output.ppm", "w+");
	fprintf(out, "P3\n%zu %zu\n255", b.w, b.h);
	for (size_t y = 0; y < b.h; y++) {
		fprintf(out, "\n");
		for (size_t x = 0; x < b.w; x++) {
			struct pixel p = b.raw[x][y];
			fprintf(out, "%u %u %u ", p.r, p.g, p.b);
		}
	}
	fclose(out);
}

void
free_buffer(struct buffer b)
{
	for (size_t i = 0; i < b.w; i++) {
		free(b.raw[i]);
	}
	free(b.raw);
}

int
main(void)
{
	struct buffer buf = new_buffer(640, 480);

	struct pixel red = {255, 0, 0};
	struct pixel green = {0, 255, 0};
	struct pixel blue = {0, 0, 255};

	for (int y = 0; y < 160; y++) {
		for (int x = 0; x < 640; x++) {
			put_pixel(buf, x, y, red);
		}
	}

	for (int y = 160; y < 320; y++) {
		for (int x = 0; x < 640; x++) {
			put_pixel(buf, x, y, green);
		}
	}

	for (int y = 320; y < 480; y++) {
		for (int x = 0; x < 640; x++) {
			put_pixel(buf, x, y, blue);
		}
	}

	write_buffer(buf);
	free_buffer(buf);
	return EXIT_SUCCESS;
}
