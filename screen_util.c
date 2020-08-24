#include "screen.h"
#include "screen_util.h"
#include "vec3f.h"

vec3f
screen_util_vec3f_size(screen* s)
{
	return (vec3f) {s->w, s->h, 0};
}
