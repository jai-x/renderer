#include <stdbool.h>
#include <math.h>

#include "vec3f.h"
#include "screen.h"
#include "screen_draw.h"
#include "util.h"

// Draw a line on the screen from (x1, y2) to (x2, y2) using Bresenham's line
// algorithm. I couldn't find a way to derive a non-buggy version myself so I
// just copied an implementation I knew was working from ssloy/tinyrenderer.
void
screen_draw_line(screen* s, int x1, int y1, int x2, int y2)
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
		if (transpose) {
			screen_set_point(s, y, x);
		} else {
			screen_set_point(s, x, y);
		}

		err += derr;

		if (err > dx) {
			y += (y2 > y1) ? 1 : -1;
			err -= dx * 2;
		}
	}
}

// Draw triangle represented by vertices `a`, `b`, and `c` on the screen
void
screen_draw_triangle(screen* scrn, vec3f a, vec3f b, vec3f c)
{
	vec3f box_min = {scrn->w - 1, scrn->h - 1, 0};
	vec3f box_max = {0, 0, 0};

	box_min = vec3f_min(box_min, a);
	box_min = vec3f_min(box_min, b);
	box_min = vec3f_min(box_min, c);

	box_max = vec3f_max(box_max, a);
	box_max = vec3f_max(box_max, b);
	box_max = vec3f_max(box_max, c);

	// point in buffer to check
	vec3f p;

	// for each point in the bounding box that surrounds the triangle
	for (p.x = box_min.x; p.x < box_max.x; p.x++) {
		for (p.y = box_min.y; p.y < box_max.y; p.y++) {
			vec3f bc = barycentric(a, b, c, p);

			// barycentric coordinates less than zero fall outside the triangle
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
				continue;
			}

			// find z buffer value of this pixel
			// sum of product of z values of the trinagle vertices and the barycentric coordinates
			// TODO: Explain why this works
			p.z = 0.0f + (a.z * bc.x) + (b.z * bc.y) + (c.z * bc.z);

			// if the new z buffer value is larger than the current one for this pixel
			if (screen_get_z(scrn, p.x, p.y) < p.z ) {
				// update to the new z value for this pixel
				screen_set_z(scrn, p.x, p.y, p.z);
				// draw the pixel
				screen_set_point(scrn, p.x, p.y);
			}
		}
	}
}
