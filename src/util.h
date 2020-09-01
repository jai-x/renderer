#pragma once

#include <stdbool.h>
#include <sys/time.h>

#include "vec2i.h"
#include "vec3f.h"

void int_swap(int* a, int* b);
vec3f barycentric(vec2i a, vec2i b, vec2i c, vec2i p);
bool within_triangle(vec2i a, vec2i b, vec2i c, vec2i p);
float a_map(float val, float a_min, float a_max, float b_min, float b_mac);
bool timeval_sub(const struct timeval* a, const struct timeval* b, struct timeval* r);
