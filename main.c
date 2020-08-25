#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "vec2i.h"
#include "vec3f.h"
#include "util.h"
#include "screen.h"
#include "screen_draw.h"

int
main(void)
{
	model* teapot = model_alloc("./obj/teapot.obj");
	screen* scrn = screen_alloc(700, 700, "renderer");

	while (screen_is_alive(scrn)) {
		// clear screen to black
		screen_set_color(scrn, 0, 0, 0, 255);
		screen_clear(scrn);

		for (int i = 0; i < teapot->num_faces; i++) {
			// fetch face
			face f = teapot->faces[i];

			// fetch coordinates of each vertex of the face triangle
			vec3f v0 = teapot->verts[f.v0 - 1];
			vec3f v1 = teapot->verts[f.v1 - 1];
			vec3f v2 = teapot->verts[f.v2 - 1];

			// map the vertices to screen space and only use the x and y coordinates
			vec2i t0 = {
				a_map(v0.x, teapot->min.x, teapot->max.x, 0, scrn->w),
				a_map(v0.y, teapot->min.y, teapot->max.y, 0, scrn->h)
			};
			vec2i t1 = {
				a_map(v1.x, teapot->min.x, teapot->max.x, 0, scrn->w),
				a_map(v1.y, teapot->min.y, teapot->max.y, 0, scrn->h)
			};
			vec2i t2 = {
				a_map(v2.x, teapot->min.x, teapot->max.x, 0, scrn->w),
				a_map(v2.y, teapot->min.y, teapot->max.y, 0, scrn->h)
			};

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
