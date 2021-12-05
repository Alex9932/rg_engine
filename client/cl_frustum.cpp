/*
 * cl_frustum.cpp
 *
 *  Created on: Dec 3, 2021
 *      Author: alex9932
 */

#include "cl_frustum.h"

void cl_calcFrustum(Frustum* f, Camera* cam) {
	float hVSide = cam->far * tan(cam->fov * 0.5);
	float hHSide = hVSide * cam->aspect;

}
