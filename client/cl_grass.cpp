/*
 * cl_grass.cpp
 *
 *  Created on: Oct 22, 2021
 *      Author: alex9932
 */

#include "cl_grass.h"
#include <rg_mesh.h>
#include "cl_vao.h"

#include "cl_surfaces.h"
#include "cl_texture.h"

static const float SIZE = 1.0;

static rg_vertex_t VERTICES[] = {
		{ -SIZE/2,    0, 0, 0, 1, 0, 1, 1, 1, 0, 1 }, // 0
		{  SIZE/2,    0, 0, 0, 1, 0, 1, 1, 1, 1, 1 }, // 1
		{  SIZE/2, SIZE, 0, 0, 1, 0, 1, 1, 1, 1, 0 }, // 2 <<
		{ -SIZE/2, SIZE, 0, 0, 1, 0, 1, 1, 1, 0, 0 }, // 3 <<

		{ 0, 0,    -SIZE/2, 0, 1, 0, 1, 1, 1, 0, 1 }, // 4
		{ 0, 0,     SIZE/2, 0, 1, 0, 1, 1, 1, 1, 1 }, // 5
		{ 0, SIZE,  SIZE/2, 0, 1, 0, 1, 1, 1, 1, 0 }, // 6 <<
		{ 0, SIZE, -SIZE/2, 0, 1, 0, 1, 1, 1, 0, 0 }  // 7 <<
};

static Uint32 INDICES[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

static cl_VAO* grass;

static cl_texture_t t_grass;
static cl_texture_t t_flower;
static cl_texture_t t_norm;


static const int GRASS_SIZE = 40;


static float offsets[GRASS_SIZE * GRASS_SIZE * 3];

void cl_grass_init() {
	rg_mesh_t mesh;
	mesh.vertices = VERTICES;
	mesh.vertex_count = 8;
	mesh.indices = INDICES;
	mesh.index_count = 12;
	grass = cl_makeVAO(&mesh, GL_STREAM_DRAW);

	t_grass = cl_getTexture("gamedata/textures/grass.png");
	t_flower = cl_getTexture("gamedata/textures/flower.png");
	t_norm = cl_getTexture("gamedata/textures/norm.png");

	int offset = 0;
	for (int x = 0; x < GRASS_SIZE; ++x) {
		for (int z = 0; z < GRASS_SIZE; ++z) {
			float offset_x = (float)(rand() % 10) * 0.1f;
			float offset_z = (float)(rand() % 10) * 0.1f;
			float rotation = math_toradians(rand() % 90);

			offsets[offset * 3 + 0] = ((x - GRASS_SIZE/2) * 0.56) + offset_x;
			offsets[offset * 3 + 1] = ((z - GRASS_SIZE/2) * 0.56) + offset_z;
			offsets[offset * 3 + 2] = rotation;
			offset++;
		}
	}
}

void cl_grass_destroy() {
	cl_deleteVAO(grass);
	cl_deleteTexture(t_grass);
	cl_deleteTexture(t_flower);
}

static double time = 0;

void cl_grass_render(double delta, rg_Shader shader) {
	time += delta;

	rg_vertex_t* new_verts = VERTICES;

//	new_verts[2].vertex.x = VERTICES[2].vertex.x + cos(time);
//	new_verts[2].vertex.z = VERTICES[2].vertex.z + sin(time);
//	new_verts[3].vertex.x = VERTICES[3].vertex.x + cos(time);
//	new_verts[3].vertex.z = VERTICES[3].vertex.z + sin(time);
//	VERTICES[3].vertex.x += SDL_sin(time);
//	VERTICES[3].vertex.z += SDL_cos(time);
//	VERTICES[6].vertex.x += SDL_sin(time);
//	VERTICES[6].vertex.z += SDL_cos(time);
//	VERTICES[7].vertex.x += SDL_sin(time);
//	VERTICES[7].vertex.z += SDL_cos(time);

	glBindBuffer(GL_ARRAY_BUFFER, grass->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rg_vertex_t) * 8, new_verts, GL_STREAM_DRAW);


	shader_uniform_1i(shader_uniform_get(shader, "surface_type"), SURFACE_GRASS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_grass);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_norm);

	glDisable(GL_CULL_FACE);

	mat4 matrix;
	for (int i = 0; i < GRASS_SIZE * GRASS_SIZE; ++i) {
		float off_x = offsets[i * 3 + 0];
		float off_z = offsets[i * 3 + 1];
		float rotat = offsets[i * 3 + 2];
		mat4_identity(&matrix);
		mat4_rotate(&matrix, 0, rotat, 0);
		mat4_translate(&matrix, off_x, 0, off_z);
		shader_uniform_mat4f(shader_uniform_get(shader, "model"), (float*)&matrix);
		cl_drawVAO(grass);
	}

	glEnable(GL_CULL_FACE);
}
