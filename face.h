#pragma once

typedef struct { int v0, v1, v2; } face;

void face_print(face* f);
face face_from_line(const char* line);
