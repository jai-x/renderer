#include <stdio.h>

#include "vec2i.h"

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

vec2i
vec2i_min_values(vec2i a, vec2i b)
{
	vec2i out;
	out.x = imin(a.x, b.x);
	out.y = imin(a.y, b.y);
	return out;
}

vec2i
vec2i_max_values(vec2i a, vec2i b)
{
	vec2i out;
	out.x = imax(a.x, b.x);
	out.y = imax(a.y, b.y);
	return out;
}
