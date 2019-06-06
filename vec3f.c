#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vec3f.h"

void
vec3f_print(vec3f v) {
	printf("x: %f, y: %f, z: %f\n", v.x, v.y, v.z);
}

// Extract a vec3f vertex struct from a line of three floats
vec3f
vec3f_from_line(const char* line)
{
	char* end;
	vec3f out;
	out.x = strtof(line, &end);
	out.y = strtof(end, &end);
	out.z = strtof(end, NULL);
	return out;
}

vec3f
vec3f_min_values(vec3f a, vec3f b)
{
	vec3f out;
	out.x = fminf(a.x, b.x);
	out.y = fminf(a.y, b.y);
	out.z = fminf(a.z, b.z);
	return out;
}

vec3f
vec3f_max_values(vec3f a, vec3f b)
{
	vec3f out;
	out.x = fmaxf(a.x, b.x);
	out.y = fmaxf(a.y, b.y);
	out.z = fmaxf(a.z, b.z);
	return out;
}

static float
scale(float old_min, float old_max, float new_min, float new_max, float val)
{
	float old_range = old_max - old_min;
	float new_range = new_max - new_min;
	return ((val - old_min) * (new_range / old_range)) + new_min;
}

vec3f
vec3f_scale(vec3f old_min, vec3f old_max, vec3f new_min, vec3f new_max, vec3f val)
{
	vec3f out;
	out.x = scale(old_min.x, old_max.x, new_min.x, new_max.x, val.x);
	out.y = scale(old_min.y, old_max.y, new_min.y, new_max.y, val.y);
	out.z = scale(old_min.z, old_max.z, new_min.z, new_max.z, val.z);
	return out;
}
