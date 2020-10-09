#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "model.h"
#include "vec3f.h"

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

// Parses string containing vertex info `line` and returns the resultant struct
static inline vec3f
parse_vertex_line(const char* line) {
	// start parsing after the "f " prefix
	char* pos = (char*) (line + 2);

	vec3f out;
	out.x = strtof(pos, &pos);
	out.y = strtof(pos, &pos);
	out.z = strtof(pos, NULL);
	return out;
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

// Parses an integer triple in four possible forms and returns how far along the
// input string has been parsed
// The four possible forms:
// 1 `v`
// 2 `v/vt`
// 3 `v/vt/vn`
// 4 `v//vn`
static size_t
parse_int_triple(const char* triple, int* v, int* vt, int* vn)
{
	char* pos = (char*) triple;

	*v = (int) strtol(pos, &pos, 10);            // Parse `v` as in form 1
	if (pos[0] == '/' && pos[1] == '/') {        // Double slash indicates form 4
		pos += 2;                                // Skip to after the slash
		*vn = (int) strtol(pos, &pos, 10);       // Parse `vn` and end
	} else if (pos[0] == '/' && pos[1] != '/') { // Single slash is form 2 or 3
		pos += 1;                                // Skip slash
		*vt = (int) strtol(pos, &pos, 10);       // Parse `vt and end if form 2
		if (pos[0] == '/') {                     // Another single slash is form 3
			pos += 1;                            // Skip slash
			*vn = (int) strtol(pos, &pos, 10);   // Parse `vn` and end
		}
	}

	// Find how far along the triple string we have moved
	return (size_t) (pos - triple);
}

// Parses string containing face info `line` and return the resultant struct
static inline face
parse_face_line(const char* line)
{
	face out;
	// start parsing after the "f " prefix
	size_t offset = 2;

	// A face line is made up of three integer triples
	offset += parse_int_triple(&line[offset], &out.v0, &out.vt0, &out.vn0);
	offset += parse_int_triple(&line[offset], &out.v1, &out.vt1, &out.vn1);
	offset += parse_int_triple(&line[offset], &out.v2, &out.vt2, &out.vn2);

	return out;
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
			vec3f v = parse_vertex_line(line);
			// assign the vertex and increment position
			out->verts[v_pos++] = v;
			// Update minimum and maximum values
			out->min = vec3f_min(out->min, v);
			out->max = vec3f_max(out->max, v);
			continue;
		}

		// line represents model face
		if (is_face_line(line)) {
			// assign the face and increment position
			out->faces[f_pos++] = parse_face_line(line);
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
