#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vec3f.h"

// Debug printing
void
vec3f_print(vec3f v) {
	printf("x: %f, y: %f, z: %f\n", v.x, v.y, v.z);
}

// Parse vec3f vertex struct from a line of three floats
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

// Minimum values from the input component scalars
// Equivalent to { min(a.x, b.x), min(a.y, b.y), min(a.z, b.z) }
vec3f
vec3f_min(vec3f a, vec3f b)
{
	vec3f out;
	out.x = fminf(a.x, b.x);
	out.y = fminf(a.y, b.y);
	out.z = fminf(a.z, b.z);
	return out;
}

// Maximum values from the input component scalars
// Equivalent to { max(a.x, b.x), max(a.y, b.y), max(a.z, b.z) }
vec3f
vec3f_max(vec3f a, vec3f b)
{
	vec3f out;
	out.x = fmaxf(a.x, b.x);
	out.y = fmaxf(a.y, b.y);
	out.z = fmaxf(a.z, b.z);
	return out;
}

// Cross product of vectors
vec3f
vec3f_cross(vec3f a, vec3f b)
{
	vec3f out;
	out.x = (a.y * b.z) - (a.z * b.y);
	out.y = (a.z * b.x) - (a.x * b.z);
	out.z = (a.x * b.y) - (a.y * b.x);
	return out;
}

// Subtract vectors (a - b)
vec3f
vec3f_sub(vec3f a, vec3f b)
{
	vec3f out;
	out.x = a.x - b.x;
	out.y = a.y - b.y;
	out.z = a.z - b.z;
	return out;
}

// Dot product of input vectors
float
vec3f_dot(vec3f a, vec3f b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

// Magnitude of vector
float
vec3f_magnitude(vec3f v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

// Normalized vector (set magnitude to 1 but maintain direction)
vec3f
vec3f_normalize(vec3f v)
{
	float length = vec3f_magnitude(v);
	vec3f out;
	out.x = v.x / length;
	out.y = v.y / length;
	out.z = v.z / length;
	return out;
}
