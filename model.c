#include <stdio.h>

#include "model.h"

void
vec3f_print(vec3f* v) {
	printf("x: %f, y: %f, z: %f\n", v->x, v->y, v->x);
}

void
face_print(face* f) {
	printf("v0: %d, v1: %d, v2: %d\n", f->v0, f->v1, f->v2);
}
