/*
 * cl_renderer.h
 *
 *  Created on: 26 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_RENDERER_H_
#define CL_RENDERER_H_

#include <rg_engine.h>
#include <rg_vecmath.h>
#include <rg_level.h>
#include "cl_camera.h"
#include "cl_font.h"

void cl_r_init();
void cl_r_destroy();

void cl_r_makeProjection(Camera* camera);
cl_font_t* cl_r_getDefaultFont();

void cl_r_canRender(bool cr);
void cl_r_doRender(double dt);

void cl_r_unloadMeshes();
void cl_r_loadMesh(rg_mesh mesh);

Camera* cl_r_getCamera();

#endif /* CL_RENDERER_H_ */
