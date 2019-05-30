#pragma once

typedef struct { float x, y, z; } vec3f;
typedef struct { int v0, v1, v2; } face;
typedef struct {
	int    num_verts;
	vec3f* verts;
	int    num_faces;
	face*  faces;
} model;

void vec3f_print(vec3f* v);
void face_print(face* f);

model* model_alloc(const char* filename);
void model_free(model* m);
void model_print(model* m);
