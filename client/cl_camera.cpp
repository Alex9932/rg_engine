/*
 * cl_camera.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_camera.h"

void cl_camera_recalcViewMatrix(cl_camera* camera) {
//	mat4 rotation, translation;
//	mat4_identity(&translation);
//	mat4_euler_rotation(&rotation, camera->rotation.x, camera->rotation.y, camera->rotation.z);
//	mat4_translate(&translation, -camera->position.x, -camera->position.y, -camera->position.z);
//	mat4_mul(&camera->view, &translation, &rotation);

//	mat4_view(&camera->view, -camera->position.x, -camera->position.y, -camera->position.z, camera->rotation.x, camera->rotation.y);
	mat4_view(&camera->view, camera->position.x, camera->position.y, camera->position.z, camera->rotation.x, camera->rotation.y);
}
