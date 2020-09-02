#include <math.h>
#include <stdbool.h>
#include <sys/time.h>

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
// represented by points `a`, `b`, and `c` only in the X and Y axis.
// In a barycentric coordinates system, a triangle ABC will have the corner
// coordintes represented as:
//   AB: (1, 0, 0)
//   BC: (0, 1, 0)
//   AC: (0, 0, 1)
// Any space within the triangle can therefore be represent respect to
// the corners, in a 3 scalar vector with each scalar between 0 and 1.
// The exact center of the triangle will have coordinates (1/3, 1/3, 1/3).
vec3f
barycentric(vec3f a, vec3f b, vec3f c, vec3f p)
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

// Determines if point `p` lies within the triangle represented by
// points `a`, `b`, and `c`.
bool
within_triangle(vec3f a, vec3f b, vec3f c, vec3f p)
{
	vec3f bc = barycentric(a, b, c, p);

	// barycentric coordinates less than zero fall outside the triangle
	return !(bc.x < 0 || bc.y < 0 || bc.z < 0);
}


// Returns an affine map for value `val`, mapping from range A (a_min to a_max)
// to range B (b_min to b_max)
float
a_map(float val, float a_min, float a_max, float b_min, float b_max)
{
	return (val - a_min) * ((b_max - b_min) / (a_max - a_min));
}


// Subracts the timeval `b` from timeval `a` and populates the results into `r`
// and returns true if the value is negative.
// Shamelessly stolen and adapted from: https://gist.github.com/vchernov/4774682
bool
timeval_sub(const struct timeval* a, const struct timeval* b, struct timeval* r)
{
	bool negative;
	if (a->tv_sec > b->tv_sec) {
		negative = false;
		if (a->tv_usec >= b->tv_usec) {
			r->tv_sec = a->tv_sec - b->tv_sec;
			r->tv_usec = a->tv_usec - b->tv_usec;
		} else {
			r->tv_sec = a->tv_sec - b->tv_sec - 1;
			r->tv_usec = 1000000 - b->tv_usec + a->tv_usec;
		}
	} else if (a->tv_sec < b->tv_sec) {
		negative = true;
		if (a->tv_usec > b->tv_usec) {
			r->tv_sec = b->tv_sec - a->tv_sec - 1;
			r->tv_usec = 1000000 - a->tv_usec + b->tv_usec;
		} else {
			r->tv_sec = b->tv_sec - a->tv_sec;
			r->tv_usec = b->tv_usec - a->tv_usec;
		}
	} else {
		r->tv_sec = 0;
		if (a->tv_usec >= b->tv_usec) {
			negative = false;
			r->tv_usec = a->tv_usec - b->tv_usec;
		} else {
			negative = true;
			r->tv_usec = b->tv_usec - a->tv_usec;
		}
	}
	return negative;
}
