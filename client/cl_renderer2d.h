/*
 * cl_renderer2d.h
 *
 *  Created on: 26 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_RENDERER2D_H_
#define CL_RENDERER2D_H_

#include <rg_engine.h>
#include <rg_vecmath.h>
#include "cl_font.h"

typedef struct cl_vertex2d_t {
	vec2 vertex;
	vec2 tex_coord;
	vec4 color;
} cl_vertex2d_t;

typedef void (*cl_r2d_drawfunc)(double);

void cl_r2d_init();
void cl_r2d_destroy();
void cl_r2d_doRender(double dt, cl_r2d_drawfunc rf);

// Transform
void cl_r2d_translate(vec2 pos);
void cl_r2d_rotate(vec3 rot);

// Geometry
void cl_r2d_begin();
void cl_r2d_end();
void cl_r2d_vertex(cl_vertex2d_t vertex);

// Misc
void cl_r2d_bind(GLuint texture);
void cl_r2d_drawString(cl_font_t* font, rg_wstring str, int x, int y, float size, float r, float g, float b, float a);
float cl_r2d_strlen(cl_font_t* font, rg_wstring str, float size);

#endif /* CL_RENDERER2D_H_ */
