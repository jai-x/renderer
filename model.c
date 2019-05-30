#include <stdio.h>
#include <stdlib.h>

#include "model.h"

#define LINE_SIZE 100

void
vec3f_print(vec3f* v) {
	printf("x: %f, y: %f, z: %f\n", v->x, v->y, v->z);
}

void
face_print(face* f) {
	printf("v0: %d, v1: %d, v2: %d\n", f->v0, f->v1, f->v2);
}

// Count the number of lines in the file that begin with the specific character
static int
find_num_first_char(const char* filename, char first)
{
	int count = 0;
	char* line = malloc(sizeof(char) * LINE_SIZE);
	FILE* model_file = fopen(filename, "r");

	while (fgets(line, LINE_SIZE, model_file)) {
		if (line[0] == first) {
			count++;
		}
	}

	fclose(model_file);
	free(line);
	return count;
}

// Extract the three float values in the vertex line
static void
line_to_floats(const char* line, float* x, float* y, float* z)
{
	char* end;
	*x = strtof(line, &end);
	*y = strtof(end, &end);
	*z = strtof(end, NULL);
}

// Extract the three integer values in the face line
static void
line_to_ints(const char* line, int *a, int* b, int* c)
{
	char* end;
	*a = strtol(line, &end, 10);
	*b = strtol(end, &end, 10);
	*c = strtol(end, NULL, 10);
}

model*
model_alloc(const char* filename)
{
	model* out = malloc(sizeof(model));

	// Count and allocate the number of vertexes
	out->num_verts = find_num_first_char(filename, 'v');
	out->verts = malloc(sizeof(vec3f) * out->num_verts);

	// Count and allocate the number of faces
	out->num_faces = find_num_first_char(filename, 'f');
	out->faces = malloc(sizeof(face) * out->num_faces);

	// Postion of structure we are filling in the model
	int v_pos = 0;
	int f_pos = 0;

	// Allocate line and open file
	char* line = malloc(sizeof(char) * LINE_SIZE);
	FILE* model_file = fopen(filename, "r");

	// Read file by line into the allocated line
	while (fgets(line, LINE_SIZE, model_file)) {
		// line represents model vertex
		if (line[0] == 'v') {
			// clear first character
			line[0] = ' ';
			// vector to allocate to
			vec3f v;
			// parse floats into vector
			line_to_floats(line, &(v.x), &(v.y), &(v.z));
			// assign the vector and increment position
			out->verts[v_pos++] = v;
			continue;
		}

		// line represents model face
		if (line[0] == 'f') {
			// clear first character
			line[0] = ' ';
			// face to allocate to
			face f;
			// parse ints into face
			line_to_ints(line, &(f.v0), &(f.v1), &(f.v2));
			// assign the face and increment position
			out->faces[f_pos++] = f;
			continue;
		}
	}

	// Close file, free line
	fclose(model_file);
	free(line);

	return out;
}

void
model_free(model* m)
{
	free(m->verts);
	free(m->faces);
	free(m);
}

void
model_print(model* m)
{
	/*
	for (int i = 0; i < m->num_verts; i++) {
		vec3f_print(&m->verts[i]);
	}

	for (int i = 0; i < m->num_faces; i++) {
		face_print(&m->faces[i]);
	}
	*/
	printf("vectors: %d, faces: %d\n", m->num_verts, m->num_faces);
}
