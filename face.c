#include <stdio.h>
#include <stdlib.h>

#include "face.h"

void
face_print(face* f) {
	printf("v0: %d, v1: %d, v2: %d\n", f->v0, f->v1, f->v2);
}

// Extract face struct from a line of three integer values
face
face_from_line(const char* line)
{
	char* end;
	face out;
	out.v0 = strtol(line, &end, 10);
	out.v1 = strtol(end, &end, 10);
	out.v2 = strtol(end, NULL, 10);
	return out;
}

