#pragma once

#include "screen.h"
#include "vec2i.h"

void screen_draw_line(screen* s, int x1, int y1, int x2, int y2);
void screen_draw_triangle(screen* s, vec2i v0, vec2i v1, vec2i v2);
