/*
 * cl_frustum.h
 *
 *  Created on: Dec 3, 2021
 *      Author: alex9932
 */

#ifndef CL_FRUSTUM_H_
#define CL_FRUSTUM_H_

#include <rg_vecmath.h>
#include <rg_engine.h>
#include "cl_camera.h"

typedef struct F_Plate {
	vec3 normal;
	float distance;
} F_Plane;

typedef struct Frustum {
	F_Plane top;
	F_Plane bottom;
	F_Plane right;
	F_Plane left;
	F_Plane far;
	F_Plane near;
} Frustum;

void cl_calcFrustum(Frustum* f, Camera* cam);

#endif /* CL_FRUSTUM_H_ */
