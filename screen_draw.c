#include <stdbool.h>
#include <math.h>

#include "screen.h"
#include "screen_draw.h"
#include "vec2i.h"
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

// Draw triangle of corner coordinates v0->v1->v2 on the screen, with line color
// and fill color `c`
void
screen_draw_triangle(screen* s, vec2i v0, vec2i v1, vec2i v2)
{
	// find a bounding box within that surrounds the triangle

	// find bounding box minimum coordinates
	vec2i box_min = {s->w - 1, s->h - 1};
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

	// for each point in the bounding box that surrounds the triangle
	for (p.x = box_min.x; p.x < box_max.x; p.x++) {
		for (p.y = box_min.y; p.y < box_max.y; p.y++) {
			// find barycentric coordinates of point p on the triangle
			vec3f bc = barycentric(v0, v1, v2, p);

			// coordinates less than zero fall outside the triangle and are skipped
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
				continue;
			}

			// this point is within the triangle, so draw it on the buffer
			screen_set_point(s, p.x, p.y);
		}
	}
}
