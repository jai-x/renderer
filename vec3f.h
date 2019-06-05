#pragma once

typedef struct { float x, y, z; } vec3f;

void vec3f_print(vec3f v);
vec3f vec3f_from_line(const char* line);
vec3f vec3f_min_values(vec3f a, vec3f b);
vec3f vec3f_max_values(vec3f a, vec3f b);
vec3f vec3f_scale(vec3f old_min, vec3f old_max, vec3f new_min, vec3f new_max, vec3f val);
