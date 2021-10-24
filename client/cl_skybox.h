/*
 * cl_skybox.h
 *
 *  Created on: Oct 18, 2021
 *      Author: alex9932
 */

#ifndef CL_SKYBOX_H_
#define CL_SKYBOX_H_

#include "cl_shader.h"

void cl_skybox_init();
void cl_skybox_destroy();
void cl_skybox_render(rg_Shader shader);

#endif /* CL_SKYBOX_H_ */
