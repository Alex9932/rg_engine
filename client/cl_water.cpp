/*
 * cl_water.cpp
 *
 *  Created on: Oct 26, 2021
 *      Author: alex9932
 */

#include "cl_water.h"
#include <rg_mesh.h>
#include "cl_vao.h"

#include "cl_surfaces.h"
#include "cl_texture.h"
#include "cl_renderer.h"

static const float SIZE = 5.0;

// Flat quad
static Uint32 INDICES[] = { 0, 1, 2, 2, 3, 0 };
static rg_vertex_t VERTICES[] = {
		{ -SIZE, 0, -SIZE, 0, 1, 0, -1, 0, 0, 0, 0 },
		{  SIZE, 0, -SIZE, 0, 1, 0, -1, 0, 0, 1, 0 },
		{  SIZE, 0,  SIZE, 0, 1, 0, -1, 0, 0, 1, 1 },
		{ -SIZE, 0,  SIZE, 0, 1, 0, -1, 0, 0, 0, 1 }
};

static cl_VAO* water;
static cl_texture_t t_norm;


static const int WATER_SIZE = 1;
static float offsets[WATER_SIZE * WATER_SIZE * 3];

void cl_water_init() {
	rg_mesh_t mesh;
	mesh.vertices = VERTICES;
	mesh.vertex_count = 4;
	mesh.indices = INDICES;
	mesh.index_count = 6;
	water = cl_makeVAO(&mesh, GL_STATIC_DRAW);

	t_norm = cl_getTexture("gamedata/textures/norm.png");
//	t_norm = cl_getTexture("gamedata/materials/metal/norm.png");

	int offset = 0;
	for (int x = 0; x < WATER_SIZE; ++x) {
		for (int z = 0; z < WATER_SIZE; ++z) {
			float offset_x = (float)(rand() % 10) * 0.1f;
			float offset_z = (float)(rand() % 10) * 0.1f;
			float rotation = math_toradians(rand() % 90);
			offset_x = 0;
			offset_z = 3;

			offsets[offset * 3 + 0] = ((x - WATER_SIZE/2) * 0.56) + offset_x;
			offsets[offset * 3 + 1] = ((z - WATER_SIZE/2) * 0.56) + offset_z;
			offsets[offset * 3 + 2] = rotation;
			offset++;
		}
	}
}

void cl_water_destroy() {
	cl_deleteVAO(water);
	cl_deleteTexture(t_norm);
}

static double _time = 0;

void cl_water_render(double delta, rg_Shader shader) {
	return;

	_time += delta;


	shader_uniform_1i(shader_uniform_get(shader, "surface_type"), SURFACE_WATER);
//	shader_uniform_1i(shader_uniform_get(shader, "surface_type"), SURFACE_DEFAULT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 4);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_norm);

	glDisable(GL_CULL_FACE);

	mat4 matrix;
	for (int i = 0; i < WATER_SIZE * WATER_SIZE; ++i) {
		float off_x = offsets[i * 3 + 0];
		float off_z = offsets[i * 3 + 1];

		float distx = cl_r_getCamera()->position.x - off_x;
		float distz = cl_r_getCamera()->position.z - off_z;

		if(sqrt(distx*distx + distz*distz) > 150) {
			continue;
		}

		float rotat = offsets[i * 3 + 2];
		mat4_identity(&matrix);
		mat4_rotate(&matrix, 0, rotat, 0);
		mat4_translate(&matrix, off_x, 1, off_z);
		shader_uniform_mat4f(shader_uniform_get(shader, "model"), (float*)&matrix);
		cl_drawVAO(water);
	}

	glEnable(GL_CULL_FACE);
}
