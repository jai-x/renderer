#pragma once

typedef struct { float x, y, z; } vec3f;

void vec3f_print(vec3f v);
vec3f vec3f_from_line(const char* line);
vec3f vec3f_min(vec3f a, vec3f b);
vec3f vec3f_max(vec3f a, vec3f b);
vec3f vec3f_scale(vec3f old_min, vec3f old_max, vec3f new_min, vec3f new_max, vec3f val);
vec3f vec3f_cross(vec3f a, vec3f b);
vec3f vec3f_sub(vec3f a, vec3f b);
float vec3f_dot(vec3f a, vec3f b);
float vec3f_magnitude(vec3f v);
vec3f vec3f_normalize(vec3f v);
