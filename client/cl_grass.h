/*
 * cl_grass.h
 *
 *  Created on: Oct 22, 2021
 *      Author: alex9932
 */

#ifndef CL_GRASS_H_
#define CL_GRASS_H_

#include "cl_shader.h"

void cl_grass_init();
void cl_grass_destroy();
void cl_grass_render(double delta, rg_Shader shader);

#endif /* CL_GRASS_H_ */
