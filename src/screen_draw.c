#include <stdbool.h>
#include <stdlib.h>
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

// Draw triangle represented by vertices `a`, `b`, and `c` on screen `scrn`
void
screen_draw_triangle(screen* scrn, vec3f a, vec3f b, vec3f c)
{
	// Iteratively the find the bottom left corner of the minimum bounding box
	// surrounding the triangle in screen space
	vec3f box_min = {scrn->w - 1, scrn->h - 1, 0};
	box_min = vec3f_min(box_min, a);
	box_min = vec3f_min(box_min, b);
	box_min = vec3f_min(box_min, c);

	// Iteratively the find the top right corner of the minimum bounding box
	// surrounding the triangle in screen space
	vec3f box_max = {0, 0, 0};
	box_max = vec3f_max(box_max, a);
	box_max = vec3f_max(box_max, b);
	box_max = vec3f_max(box_max, c);

	vec3f p;

	// For each point in the minimum bounding box that surrounds the triangle
	for (p.x = box_min.x; p.x < box_max.x; p.x++) {
		for (p.y = box_min.y; p.y < box_max.y; p.y++) {
			// Find the barycentric coordinates of this point
			vec3f bc = barycentric(a, b, c, p);

			// If the barycentric coordinates are less than zero fall, this
			// point lies outside the triangle, so skip drawing for this point
			if (bc.x < 0 || bc.y < 0 || bc.z < 0) {
				continue;
			}

			// Barycentric coordinates of the point in the triangle can be
			// viewed as the weighted sum of the point in releation to all of
			// the vertices. As such, finding the z component of the point is
			// a linear combination of the z components of the vertices and the
			// barycentric coordinates of the point.

			// As an implementation detail, since the barycentric coordinate is
			// represented as type vec3f, it is not immediately clear which
			// vector component is representative of which triangle vertex.
			// For a triangle with vertices ABC which have been passed as
			// arguments to obtain the barycentric vec3f coordinate in the same
			// order, the relation applies as follows:
			//   Vertex A <=> First Barycentric component (x)
			//   Vertex B <=> Second Barycentric component (y)
			//   Vertex C <=> Third Barycentric component (z)

			// Calculate linear combination of the vertex z components and the
			// barycentric coordinate to get the z components of the point.
			p.z = 0.0f + (a.z * bc.x) + (b.z * bc.y) + (c.z * bc.z);
			//            ^        ^     ^        ^     ^        ^
			//            |        |     |        |     |        |
			//            |        |     |        |     ------------> Vertex C <=> Third Barycentric component (z)
			//            |        |     ---------------------------> Vertex B <=> Second Barycentric component (y)
			//            ------------------------------------------> Vertex A <=> First Barycentric component (x)

			// Check the screen z buffer to see if the z component value for
			// this point is larger than the current one. If it is, then this
			// point lies infront of the previous drawn point in this position
			// and can be drawn over.
			if (screen_get_z(scrn, (int) p.x, (int) p.y) < p.z ) {
				// Update the z buffer to the new z value for this point
				screen_set_z(scrn, (int) p.x, (int) p.y, p.z);
				// Draw the point on the screen
				screen_set_point(scrn, (int) p.x, (int) p.y);
			}
		}
	}
}
