#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <sys/time.h>
#include <sys/resource.h>

#include "model.h"
#include "vec2i.h"
#include "vec3f.h"
#include "util.h"
#include "screen.h"
#include "screen_draw.h"

static const vec3f light_direction = {0, 0, -1};

// Scales `v` from the model space in `mdl` to the screen space in `scrn`.
// This is done via an affine map on the x and y coordinates of `v`.
// Since we are using the max ranges of both model space and screen space, this
// results in the centering and maximizing the model within screen space.
// Since we are currently only doing a simple 2D orthographic projection without
// considering perspective, this operation does not affect the z axis at all.
// Since the output vec3f is expected in screen space, the resultant coordinates
// must be integer values.
static inline vec3f
model_to_screen(screen* scrn, model* mdl, vec3f v)
{
	return (vec3f) {
		floorf( a_map(v.x, mdl->min.x, mdl->max.x, 0, (float) scrn->w) ),
		floorf( a_map(v.y, mdl->min.y, mdl->max.y, 0, (float) scrn->h) ),
		v.z
	};
}

static inline void
render(screen* scrn, model* mdl)
{
	for (size_t i = 0; i < mdl->num_faces; i++) {
		// fetch face
		face f = mdl->faces[i];

		// fetch coordinates of each vertex of the face triangle
		vec3f v0 = mdl->verts[f.v0 - 1];
		vec3f v1 = mdl->verts[f.v1 - 1];
		vec3f v2 = mdl->verts[f.v2 - 1];

		// map the vertices to screen space
		vec3f t0 = model_to_screen(scrn, mdl, v0);
		vec3f t1 = model_to_screen(scrn, mdl, v1);
		vec3f t2 = model_to_screen(scrn, mdl, v2);

		// normal of the triangle, from the cross product of two sides
		vec3f t_norm = vec3f_cross(vec3f_sub(v2, v0), vec3f_sub(v1, v0));

		// normalise the normal vector
		t_norm = vec3f_normalize(t_norm);

		// light intensity of triangle
		float t_intensity = vec3f_dot(t_norm, light_direction);

		// negative light intensity means the triangle is invisble/not reachable by light
		if (t_intensity > 0) {
			// set color depending on light intensity
			int rgb = (int) (255 * t_intensity);
			screen_set_color(scrn, rgb, rgb, rgb, 255);

			// draw the triangle
			screen_draw_triangle(scrn, t0, t1, t2);
		}
	}
}

static void
timed_render(screen* scrn, model* mdl)
{
	// structs
	struct rusage stats;
	struct timeval start, end, elapsed;

	// get system time usage before
	getrusage(RUSAGE_SELF, &stats);
	start = stats.ru_utime;

	render(scrn, mdl);

	// get system time usage after
	getrusage(RUSAGE_SELF, &stats);
	end = stats.ru_utime;

	// subtract
	timeval_sub(&start, &end, &elapsed);

	debug("time elapsed: %lu.%06lus\n", elapsed.tv_sec, elapsed.tv_usec);
}

int
main(void)
{
	model* teapot = model_alloc("./obj/teapot.obj");
	screen* scrn = screen_alloc(800, 600);

	timed_render(scrn, teapot);

	screen_write_png(scrn, "teapot.png");

	screen_free(scrn);
	model_free(teapot);

	return EXIT_SUCCESS;
}
