#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "vec2i.h"
#include "vec3f.h"
#include "util.h"
#include "screen.h"
#include "screen_draw.h"

static const vec3f light_direction = {0, 0, -1};

void
render(screen* scrn, model* mdl)
{
	// clear screen to black
	screen_set_color(scrn, 0, 0, 0, 255);
	screen_clear(scrn);

	for (int i = 0; i < mdl->num_faces; i++) {
		// fetch face
		face f = mdl->faces[i];

		// fetch coordinates of each vertex of the face triangle
		vec3f v0 = mdl->verts[f.v0 - 1];
		vec3f v1 = mdl->verts[f.v1 - 1];
		vec3f v2 = mdl->verts[f.v2 - 1];

		// map the vertices to screen space and only use the x and y coordinates
		vec2i t0 = {
			a_map(v0.x, mdl->min.x, mdl->max.x, 0, scrn->w),
			a_map(v0.y, mdl->min.y, mdl->max.y, 0, scrn->h)
		};
		vec2i t1 = {
			a_map(v1.x, mdl->min.x, mdl->max.x, 0, scrn->w),
			a_map(v1.y, mdl->min.y, mdl->max.y, 0, scrn->h)
		};
		vec2i t2 = {
			a_map(v2.x, mdl->min.x, mdl->max.x, 0, scrn->w),
			a_map(v2.y, mdl->min.y, mdl->max.y, 0, scrn->h)
		};

		// normal of the triangle, from the cross product of two sides
		vec3f t_norm = vec3f_cross(vec3f_sub(v2, v0), vec3f_sub(v1, v0));

		// normalise the normal vector
		t_norm = vec3f_normalize(t_norm);

		// light intensity of triangle
		float t_intensity = vec3f_dot(t_norm, light_direction);

		// negative light intensity means the triangle is invisble/not reachable by light
		if (t_intensity > 0) {
			// set color depending on light intensity
			screen_set_color(scrn, 255 * t_intensity, 255 * t_intensity, 255 * t_intensity, 255);

			// draw the triangle
			screen_draw_triangle(scrn, t0, t1, t2);
		}
	}
}

int
main(void)
{
	model* teapot = model_alloc("./obj/teapot.obj");
	screen* scrn = screen_alloc(800, 600, "renderer");

	// intial render
	render(scrn, teapot);

	while (true) {
		screen_event e = screen_check_events(scrn);

		if (e == SCREEN_QUIT) {
			break;
		}

		if (e == SCREEN_REDRAW) {
			render(scrn, teapot);
		}

		screen_present(scrn);
	}

	screen_free(scrn);
	model_free(teapot);

	return EXIT_SUCCESS;
}
