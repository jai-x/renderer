#include <math.h>

#include "util.h"
#include "vec2i.h"
#include "vec3f.h"

// Swaps the value of the two provided integer pointers
void
int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Finds barycentric coordinates of point `p` with respect to triangle
// represented by points `a`, `b`, and `c`.
// In a barycentric coordinates system, a triangle ABC will have the corner
// coordintes represented as:
//   AB: (1, 0, 0)
//   BC: (0, 1, 0)
//   AC: (0, 0, 1)
// Any space within the triangle can therefore be represent respect to
// the corners, in a 3 scalar vector with each scalar between 0 and 1.
// The exact center of the triangle will have coordinates (1/3, 1/3, 1/3).
vec3f
barycentric(vec2i a, vec2i b, vec2i c, vec2i p)
{
	vec3f out = {0, 0, 0};

	// x and y of vectors A->C, A->B, and A->P
	vec3f ux = {c.x - a.x, b.x - a.x, a.x - p.x};
	vec3f uy = {c.y - a.y, b.y - a.y, a.y - p.y};

	vec3f u = vec3f_cross(ux, uy);

	// check for degenerate triangle
	// (where the points line up and it looks like a line instead of triangle)
	if (fabsf(u.z) < 1) {
		// set output coordinates outside triangle
		out = (vec3f) { -1, -1, -1 };
		return out;
	}

	out.x = 1.0f - ((u.x + u.y) / u.z);
	out.y = u.y / u.z;
	out.z = u.x / u.z;
	return out;
}
