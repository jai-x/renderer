#pragma once

#include "screen.h"
#include "vec3f.h"

void screen_draw_line(screen* s, int x1, int y1, int x2, int y2);
void screen_draw_triangle(screen* s, vec3f a, vec3f b, vec3f c);
