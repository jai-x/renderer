#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "model.h"
#include "vec3f.h"

#define IMG_X 1000
#define IMG_Y 1000

int
main(void)
{
	model* teapot = model_alloc("teapot.obj");
	buffer* img = buffer_alloc(IMG_X, IMG_Y);

	color white   = {255, 255, 255};
	vec3f img_min = {0, 0, 0};
	vec3f img_max = {IMG_X, IMG_Y, 0};

	for (int i = 0; i < teapot->num_faces; i++) {
		// fetch face
		face f = teapot->faces[i];

		// fetch coordinates of each vertex of the face triangle
		vec3f v0 = teapot->verts[f.v0 - 1];
		vec3f v1 = teapot->verts[f.v1 - 1];
		vec3f v2 = teapot->verts[f.v2 - 1];

		// scale the coordinates to the size of the buffer
		v0 = vec3f_scale(teapot->min, teapot->max, img_min, img_max, v0);
		v1 = vec3f_scale(teapot->min, teapot->max, img_min, img_max, v1);
		v2 = vec3f_scale(teapot->min, teapot->max, img_min, img_max, v2);

		// draw the triangle
		buffer_triangle(img, v0, v1, v2, white);
	}

	model_print(teapot);
	buffer_write(img, "output.ppm");
	buffer_free(img);
	model_free(teapot);

	return EXIT_SUCCESS;
}
