/*
 * cl_skybox.cpp
 *
 *  Created on: Oct 18, 2021
 *      Author: alex9932
 */

#include "cl_skybox.h"
#include <rg_mesh.h>
#include "cl_vao.h"
#include "cl_surfaces.h"
#include "cl_renderer.h"

static const float SIZE = 500;

static rg_vertex_t VERTICES[] = {
	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },

	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },

	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },

	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },

	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE,  SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },

	{ -SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE, -SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{ -SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
	{  SIZE, -SIZE,  SIZE, 0, 1, 0, 1, 1, 1, 1, 1 },
};

static Uint32 INDICES[] = {
	 0,  1,  2,  3,  4,  5,
	 6,  7,  8,  9, 10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
};

static cl_VAO* skybox;
static mat4 sb_matrix;

void cl_skybox_init() {
	rg_mesh_t mesh;
	mesh.vertices = VERTICES;
	mesh.vertex_count = 36;
	mesh.indices = INDICES;
	mesh.index_count = 36;
	skybox = cl_makeVAO(&mesh);
	mat4_identity(&sb_matrix);
}

void cl_skybox_destroy() {
	cl_deleteVAO(skybox);
}

void cl_skybox_render(rg_Shader shader) {
	shader_uniform_1i(shader_uniform_get(shader, "surface_type"), SURFACE_SKYBOX);

//	mat4_translate(&sb_matrix, cl_r_getCamera()->position.x, cl_r_getCamera()->position.y, cl_r_getCamera()->position.z);
	shader_uniform_mat4f(shader_uniform_get(shader, "model"), (float*)&sb_matrix);

	cl_drawVAO(skybox);
}
