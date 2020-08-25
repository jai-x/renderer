#pragma once

#include <stdbool.h>

#include "vec2i.h"
#include "vec3f.h"

void int_swap(int* a, int* b);
vec3f barycentric(vec2i a, vec2i b, vec2i c, vec2i p);
bool within_triangle(vec2i a, vec2i b, vec2i c, vec2i p);
