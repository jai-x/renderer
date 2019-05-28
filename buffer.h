#pragma once

typedef struct { int r, g, b; } color;
typedef struct { int w, h; color** raw; } buffer;

buffer* buffer_alloc(int w, int h);
void buffer_free(buffer* b);
void buffer_write(buffer* b, const char* filename);
void buffer_put_color(buffer* b, int x, int y, color p);
void buffer_line(buffer* b, int x1, int y1, int x2, int y2, color c);
