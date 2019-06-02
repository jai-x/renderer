#include <stdio.h>
#include <stdlib.h>

#include "model.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
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

// Extract a vec3f vertex struct from a line of three floats
static vec3f
line_to_vec3f(const char* line)
{
	char* end;
	vec3f out;
	out.x = strtof(line, &end);
	out.y = strtof(end, &end);
	out.z = strtof(end, NULL);
	return out;
}

// Extract face struct from a line of three integer values
static face
line_to_face(const char* line)
{
	char* end;
	face out;
	out.v0 = strtol(line, &end, 10);
	out.v1 = strtol(end, &end, 10);
	out.v2 = strtol(end, NULL, 10);
	return out;
}

static vec3f
vec3f_min_values(vec3f a, vec3f b)
{
	vec3f out;
	out.x = MIN(a.x, b.x);
	out.y = MIN(a.y, b.y);
	out.z = MIN(a.z, b.z);
	return out;
}

static vec3f
vec3f_max_values(vec3f a, vec3f b)
{
	vec3f out;
	out.x = MAX(a.x, b.x);
	out.y = MAX(a.y, b.y);
	out.z = MAX(a.z, b.z);
	return out;
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

	vec3f zero = {0.0, 0.0, 0.0};
	// Give zero values to min and max vec3f structs
	out->min = zero;
	out->max = zero;

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
			vec3f v = line_to_vec3f(line);
			// assign the vertex and increment position
			out->verts[v_pos++] = v;
			// Update minimum and maximum values
			out->min = vec3f_min_values(out->min, v);
			out->max = vec3f_max_values(out->max, v);
			continue;
		}

		// line represents model face
		if (line[0] == 'f') {
			// clear first character
			line[0] = ' ';
			// face to allocate to
			face f = line_to_face(line);
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
	printf("vertexes: %d, faces: %d\n", m->num_verts, m->num_faces);
	printf("min vertex: "); vec3f_print(&m->min);
	printf("max vertex: "); vec3f_print(&m->max);
}

static float
scale(float old_min, float old_max, float new_min, float new_max, float val)
{
	float old_range = old_max - old_min;
	float new_range = new_max - new_min;
	return ((val - old_min) * (new_range / old_range)) + new_min;
}

vec3f
model_scale_coords(model *m, vec3f new_min, vec3f new_max, vec3f val)
{
	vec3f out;
	out.x = scale(m->min.x, m->max.x, new_min.x, new_max.x, val.x);
	out.y = scale(m->min.y, m->max.y, new_min.y, new_max.y, val.y);
	out.z = scale(m->min.z, m->max.z, new_min.z, new_max.z, val.z);
	return out;
}
