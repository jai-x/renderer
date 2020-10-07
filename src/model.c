#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "model.h"
#include "vec3f.h"
#include "face.h"

#define LINE_SIZE 100

// Returns true if line has prefix "v "
static inline bool
is_vertex_line(const char* line)
{
	if (strlen(line) < 2) {
		return false;
	}

	return (line[0] == 'v') && (line[1] == ' ');
}

static inline void
parse_vertex_line(model* m, const char* line, size_t pos) {
	// start parsing after the "v " prefix
	vec3f v = vec3f_from_line(&line[2]);
	// assign the vertex and increment position
	m->verts[pos] = v;
	// Update minimum and maximum values
	m->min = vec3f_min(m->min, v);
	m->max = vec3f_max(m->max, v);
}

// Returns true if line has prefix "f "
static inline bool
is_face_line(const char* line)
{
	if (strlen(line) < 2) {
		return false;
	}

	return (line[0] == 'f') && (line[1] == ' ');
}

static inline void
parse_face_line(model* m, const char* line, size_t pos)
{
	// start parsing after the "f " prefix
	m->faces[pos] = face_from_line(&line[2]);
}


// Open a file from filename and parse as wavefront object file and allocate
// model structures to contain model features.
model*
model_alloc(const char* filename)
{
	model* out = malloc(sizeof(model));

	// Allocate line and open file
	char* line = malloc(sizeof(char) * LINE_SIZE);
	FILE* model_file = fopen(filename, "r");

	out->num_verts = 0;
	out->num_faces = 0;

	// Count the number of vertexes and faces
	while (fgets(line, LINE_SIZE, model_file)) {
		// line represents model vertex
		if (is_vertex_line(line)) {
			out->num_verts++;
			continue;
		}

		// line represents model face
		if (is_face_line(line)) {
			out->num_faces++;
			continue;
		}
	}

	// Rewind to beginning for next pass
	rewind(model_file);

	// Allocate the number of vertexes and faces
	out->verts = malloc(sizeof(vec3f) * out->num_verts);
	out->faces = malloc(sizeof(face) * out->num_faces);

	// Give zero values to min and max vec3f structs
	out->min = (vec3f) {0, 0, 0};
	out->max = (vec3f) {0, 0, 0};

	// Postion of structure we are filling in the model
	size_t v_pos = 0;
	size_t f_pos = 0;

	// Parse and construct vertexes and faces
	while (fgets(line, LINE_SIZE, model_file)) {
		// line represents model vertex
		if (is_vertex_line(line)) {
			parse_vertex_line(out, line, v_pos++);
			continue;
		}

		// line represents model face
		if (is_face_line(line)) {
			parse_face_line(out, line, f_pos++);
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

// Debug printing
void
model_print(model* m)
{
	printf("vertexes: %zu, faces: %zu\n", m->num_verts, m->num_faces);
	printf("min vertex: "); vec3f_print(m->min);
	printf("max vertex: "); vec3f_print(m->max);
}
