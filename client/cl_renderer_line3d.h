/*
 * cl_renderer_line3d.h
 *
 *  Created on: Nov 30, 2021
 *      Author: alex9932
 */

#ifndef CL_RENDERER_LINE3D_H_
#define CL_RENDERER_LINE3D_H_

#include <rg_vecmath.h>
#include "cl_camera.h"

typedef struct cl_vertex3d_t {
	vec3 vertex;
	vec4 color;
} cl_vertex3d_t;

typedef void (*cl_r3d_drawfunc)(double);

void cl_rl3d_init();
void cl_rl3d_destroy();
void cl_rl3d_doRender(double dt, cl_r3d_drawfunc rf, Camera* cam);

void cl_rl3d_applyMatrix(mat4* matrix);
void cl_rl3d_begin();
void cl_rl3d_end();
void cl_rl3d_vertex(cl_vertex3d_t vertex);

#endif /* CL_RENDERER_LINE3D_H_ */
