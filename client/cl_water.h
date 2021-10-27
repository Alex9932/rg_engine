/*
 * cl_grass.h
 *
 *  Created on: Oct 22, 2021
 *      Author: alex9932
 */

#ifndef CL_WATER_H_
#define CL_WATER_H_

#include "cl_shader.h"

void cl_water_init();
void cl_water_destroy();
void cl_water_render(double delta, rg_Shader shader);

#endif /* CL_WATER_H_ */
