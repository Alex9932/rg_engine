/*
 * cl_renderer2d.cpp
 *
 *  Created on: 26 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_renderer2d.h"
#include "cl_shader.h"
#include "cl_display.h"
#include <rg_vecmath.h>
#include <GL/glew.h>

#define RG_GUI_MAX_VERTECES 1024

static rg_Shader gui_shader;
static GLuint vao;
static GLuint vbo;
static GLuint g_vertex;
static GLuint default_texture;
static mat4 t_matrix;
static vec2 pos;
static vec3 rot;
static int size[2] = {0, 0};

void cl_r2d_init() {
	gui_shader = shader_create("platform/shader/gui.vs", "platform/shader/gui.fs", NULL);

	glGenTextures(1, &default_texture);
	glBindTexture(GL_TEXTURE_2D, default_texture);
	int data = 0xFFFFFFFF;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cl_vertex2d_t) * RG_GUI_MAX_VERTECES, NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cl_vertex2d_t) * RG_GUI_MAX_VERTECES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(cl_vertex2d_t), (GLvoid*)(sizeof(float) * 0)); // vertex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(cl_vertex2d_t), (GLvoid*)(sizeof(float) * 2)); // uv
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(cl_vertex2d_t), (GLvoid*)(sizeof(float) * 4)); // color
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	mat4_identity(&t_matrix);
	cl_font_init();
}

void cl_r2d_destroy() {
	shader_delete(gui_shader);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void cl_r2d_doRender(double dt, cl_r2d_drawfunc rf) {
	glDisable(GL_DEPTH_TEST);
	shader_start(gui_shader);
	size[0] = (int)cl_display_getWidth();
	size[1] = (int)cl_display_getHeight();
	shader_uniform_2ip(shader_uniform_get(gui_shader, "size"), size);
	rf(dt);
}

/////

void cl_r2d_translate(vec2 position) {
	pos.x = position.x;
	pos.y = position.y;
}

void cl_r2d_rotate(vec3 rotation) {
	rot.x = rotation.x;
	rot.y = rotation.y;
	rot.z = rotation.z;
}

void cl_r2d_begin() {
	g_vertex = 0;
	glBindVertexArray(vao);
}

void cl_r2d_end() {
	mat4_model(&t_matrix, pos.x, pos.y, 0, rot.x, rot.y, rot.z);
	shader_uniform_mat4f(shader_uniform_get(gui_shader, "model"), (float*)&t_matrix);
	glDrawArrays(GL_TRIANGLES, 0, g_vertex);
}

void cl_r2d_vertex(cl_vertex2d_t vertex) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cl_vertex2d_t) * g_vertex, sizeof(cl_vertex2d_t), &vertex);
	g_vertex++;
}

void cl_r2d_bind(GLuint texture) {
	if(texture == 0) { texture = default_texture; }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void cl_r2d_drawString(cl_font_t* font, rg_wstring str, int x, int y, float size, float r, float g, float b, float a) {
	shader_uniform_1i(shader_uniform_get(gui_shader, "is_text"), 1);
	cl_r2d_bind(font->atlas);
	cl_r2d_begin();
	for (Uint32 i = 0; i < wcslen(str); ++i) {
		wchar_t c = str[i];
		float xpos = x + font->chars[c].bearing_x * size;
		float ypos = y - (font->chars[c].size_y - font->chars[c].bearing_y) * size;
		float w = font->chars[c].size_x * size;
		float h = font->chars[c].size_y * size;
		float u = (float)(c % F_ATLAS_WIDTH) / (float)F_ATLAS_WIDTH;
		float v = (float)(c / F_ATLAS_WIDTH) / (float)F_ATLAS_HEIGHT;
		float su = (float)font->chars[c].size_x / (float)(F_ATLAS_WIDTH  * font->f_size);
		float sv = (float)font->chars[c].size_y / (float)(F_ATLAS_HEIGHT * font->f_size);
		cl_r2d_vertex({xpos    , ypos + h, u     , v     , r, g, b, a});
		cl_r2d_vertex({xpos    , ypos    , u     , v + sv, r, g, b, a});
		cl_r2d_vertex({xpos + w, ypos    , u + su, v + sv, r, g, b, a});
		cl_r2d_vertex({xpos    , ypos + h, u     , v     , r, g, b, a});
		cl_r2d_vertex({xpos + w, ypos    , u + su, v + sv, r, g, b, a});
		cl_r2d_vertex({xpos + w, ypos + h, u + su, v     , r, g, b, a});
		x += (font->chars[c].advance_x >> 6) * size;
	}
	cl_r2d_end();
	shader_uniform_1i(shader_uniform_get(gui_shader, "is_text"), 0);
}

float cl_r2d_strlen(cl_font_t* font, rg_wstring str, float size) {
	float len = 0;
	for (Uint32 i = 0; i < wcslen(str); ++i) {
		len += (font->chars[str[i]].advance_x >> 6) * size;
	}
	return len;
}
