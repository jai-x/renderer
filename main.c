#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "vec2i.h"
#include "vec3f.h"
#include "screen.h"
#include "screen_draw.h"

#define IMG_X 700
#define IMG_Y 700

static const vec3f img_min = {0, 0, 0};
static const vec3f img_max = {IMG_X, IMG_Y, 0};

int
main(void)
{
	model* teapot = model_alloc("teapot.obj");
	screen* scrn = screen_alloc(IMG_X, IMG_Y, "renderer");

	while (screen_is_alive(scrn)) {
		screen_set_color(scrn, 0, 0, 0, 255);
		screen_clear(scrn);

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
			screen_set_color(scrn, rand() % 255, rand() % 255, rand() % 255, 255);

			// draw the triangle
			screen_draw_triangle(scrn, t0, t1, t2);
		}

		screen_present(scrn);
	}

	screen_free(scrn);
	model_free(teapot);

	return EXIT_SUCCESS;
}
