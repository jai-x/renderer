#pragma once

#include "face.h"
#include "vec3f.h"

typedef struct {
	int    num_verts;
	vec3f* verts;
	vec3f  min, max;
	int    num_faces;
	face*  faces;
} model;

model* model_alloc(const char* filename);
void model_free(model* m);
void model_print(model* m);
vec3f model_scale_coords(model *m, vec3f new_min, vec3f new_max, vec3f val);
