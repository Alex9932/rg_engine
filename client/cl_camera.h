/*
 * cl_camera.h
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_CAMERA_H_
#define CL_CAMERA_H_

#include <rg_vecmath.h>

typedef struct cl_camera {
	vec3 position;
	vec3 rotation;
	mat4 projection;
	mat4 view;
} cl_camera;

void cl_camera_recalcViewMatrix(cl_camera* camera);

#endif /* CL_CAMERA_H_ */
