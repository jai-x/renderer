#include <stdio.h>

#include "vec2i.h"

// Debug printing
void
vec2i_print(vec2i v)
{
	printf("x: %d, y: %d\n", v.x, v.y);
}

static inline int
imin(int a, int b)
{
	return (a < b) ? a : b;
}

static inline int
imax(int a, int b)
{
	return (a > b) ? a : b;
}

// Minimum values from the input component scalars.
// Equivalent to { min(a.x, b.x), min(a.y, b.y) }
vec2i
vec2i_min_values(vec2i a, vec2i b)
{
	vec2i out;
	out.x = imin(a.x, b.x);
	out.y = imin(a.y, b.y);
	return out;
}

// Maximum values from the input component scalars
// Equivalent to { max(a.x, b.x), max(a.y, b.y) }
vec2i
vec2i_max_values(vec2i a, vec2i b)
{
	vec2i out;
	out.x = imax(a.x, b.x);
	out.y = imax(a.y, b.y);
	return out;
}
