#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "model.h"
#include "vec3f.h"

#define IMG_X 1000
#define IMG_Y 1000

static const vec3f img_min = {0, 0, 0};
static const vec3f img_max = {IMG_X, IMG_Y, 0};

int
main(void)
{
	model* teapot = model_alloc("teapot.obj");
	buffer* img = buffer_alloc(IMG_X, IMG_Y);

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

		// use only the x and y components as integers
		vec2i t0 = {v0.x, v0.y};
		vec2i t1 = {v1.x, v1.y};
		vec2i t2 = {v2.x, v2.y};

		// generate a random color for this face
		color rand_color = {rand() % 255, rand() % 255, rand() % 255};

		// draw the triangle
		buffer_triangle(img, t0, t1, t2, rand_color);
	}

	model_print(teapot);
	buffer_write(img, "output.ppm");
	buffer_free(img);
	model_free(teapot);

	return EXIT_SUCCESS;
}
