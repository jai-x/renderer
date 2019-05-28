#pragma once

typedef struct { float x, y, z; } vec3f;
typedef struct { int v0, v1, v2; } face;
typedef struct {
	int    num_vectors;
	vec3f* vectors;
	int    num_faces;
	face*  faces;
} model;

void vec3f_print(vec3f* v);
void face_print(face* f);
