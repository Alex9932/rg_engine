/*
 * cl_camera.h
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_CAMERA_H_
#define CL_CAMERA_H_

#include <rg_vecmath.h>

typedef struct Camera {
	vec3 position;
	vec3 rotation;
	mat4 projection;
	mat4 view;
	float aspect;
	float near;
	float far;
	float fov;
} Camera;

void cl_camera_recalcViewMatrix(Camera* camera);
void cl_camera_update(Camera* camera);


#endif /* CL_CAMERA_H_ */
