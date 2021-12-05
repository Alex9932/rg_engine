/*
 * cl_renderer_line3d.cpp
 *
 *  Created on: Nov 30, 2021
 *      Author: alex9932
 */

#include "cl_renderer_line3d.h"

#include "cl_shader.h"

#define RG_L3D_MAX_VERTECES 1024

static rg_Shader l3d_shader;
static GLuint vao;
static GLuint vbo;

static mat4 g_model;
static GLuint g_vertex;

void cl_rl3d_init() {
	glLineWidth(1.2);

	l3d_shader = shader_create("platform/shader/l3d.vs", "platform/shader/l3d.fs", NULL);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cl_vertex3d_t) * RG_L3D_MAX_VERTECES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cl_vertex3d_t), (GLvoid*)(sizeof(float) * 0)); // vertex
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(cl_vertex3d_t), (GLvoid*)(sizeof(float) * 3)); // color
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void cl_rl3d_destroy() {
	shader_delete(l3d_shader);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void cl_rl3d_doRender(double dt, cl_r3d_drawfunc rf, Camera* cam) {
	glDisable(GL_DEPTH_TEST);
	shader_start(l3d_shader);
	shader_uniform_mat4f(shader_uniform_get(l3d_shader, "proj"), (float*)&cam->projection);
	shader_uniform_mat4f(shader_uniform_get(l3d_shader, "view"), (float*)&cam->view);
	rf(dt);
}

void cl_rl3d_begin() {
	g_vertex = 0;
	mat4_identity(&g_model);
	glBindVertexArray(vao);
}

void cl_rl3d_applyMatrix(mat4* matrix) {
	shader_uniform_mat4f(shader_uniform_get(l3d_shader, "model"), (float*)matrix);
}

void cl_rl3d_end() {
	glDrawArrays(GL_LINES, 0, g_vertex);
}

void cl_rl3d_vertex(cl_vertex3d_t vertex) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cl_vertex3d_t) * g_vertex, sizeof(cl_vertex3d_t), &vertex);
	g_vertex++;
}

