#include <stdio.h>
#include <stdlib.h>

#include "model.h"
#include "vec3f.h"
#include "face.h"

#define LINE_SIZE 100

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

	// Give zero values to min and max vec3f structs
	out->min = (vec3f) {0, 0, 0};
	out->max = (vec3f) {0, 0, 0};

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
			// get the vertex from the line
			vec3f v = vec3f_from_line(line);
			// assign the vertex and increment position
			out->verts[v_pos++] = v;
			// Update minimum and maximum values
			out->min = vec3f_min(out->min, v);
			out->max = vec3f_max(out->max, v);
			continue;
		}

		// line represents model face
		if (line[0] == 'f') {
			// clear first character
			line[0] = ' ';
			// get face from line, assign, and increment position
			out->faces[f_pos++] = face_from_line(line);
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
	free(m->faces); free(m);
}

void
model_print(model* m)
{
	printf("vertexes: %d, faces: %d\n", m->num_verts, m->num_faces);
	printf("min vertex: "); vec3f_print(m->min);
	printf("max vertex: "); vec3f_print(m->max);
}
