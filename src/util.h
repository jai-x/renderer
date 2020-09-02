#pragma once

#include <stdbool.h>
#include <sys/time.h>

#include "vec3f.h"

void int_swap(int* a, int* b);
vec3f barycentric(vec3f a, vec3f b, vec3f c, vec3f p);
bool within_triangle(vec3f a, vec3f b, vec3f c, vec3f p);
float a_map(float val, float a_min, float a_max, float b_min, float b_mac);
bool timeval_sub(const struct timeval* a, const struct timeval* b, struct timeval* r);
