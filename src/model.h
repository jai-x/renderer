#pragma once

#include <stddef.h>

#include "vec3f.h"

typedef struct {
	int v0, vt0, vn0;
	int v1, vt1, vn1;
	int v2, vt2, vn2;
} face;

typedef struct {
	vec3f  min, max;

	size_t num_verts;
	vec3f* verts;

	size_t num_faces;
	face*  faces;
} model;

model* model_alloc(const char* filename);
void model_free(model* m);
void model_print(model* m);
vec3f model_scale_coords(model *m, vec3f new_min, vec3f new_max, vec3f val);
