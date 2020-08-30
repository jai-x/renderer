#pragma once

typedef struct { int x, y; } vec2i;

void vec2i_print(vec2i v);
vec2i vec2i_min_values(vec2i a, vec2i b);
vec2i vec2i_max_values(vec2i a, vec2i b);
