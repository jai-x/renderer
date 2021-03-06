#pragma once

typedef struct { float x, y, z; } vec3f;

#define vec3f_zero (vec3f) {0, 0, 0}

void vec3f_print(vec3f v);
vec3f vec3f_from_line(const char* line);
vec3f vec3f_min(vec3f a, vec3f b);
vec3f vec3f_max(vec3f a, vec3f b);
vec3f vec3f_cross(vec3f a, vec3f b);
vec3f vec3f_sub(vec3f a, vec3f b);
float vec3f_dot(vec3f a, vec3f b);
float vec3f_magnitude(vec3f v);
vec3f vec3f_normalize(vec3f v);
