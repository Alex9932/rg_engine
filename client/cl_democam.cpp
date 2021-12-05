/*
 * cl_democam.cpp
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#include "cl_democam.h"
#include <rg_engine.h>
#include <rg_demo.h>

static Camera* _camera;
static vec3 origin_pos;
static vec3 origin_rot;

void cl_democam_init(Camera* cam) {
	_camera = cam;
}

void cl_democam_update() {
	rg_demo_frame* frame = rg_demo_currentFrame();
	_camera->position = frame->position;
	_camera->rotation = frame->rotation;
}

void cl_democam_start() {
	// Save origin camera's position
	origin_pos = _camera->position;
	origin_rot = _camera->rotation;
}

void cl_democam_end() {
	// Restore position
	_camera->position = origin_pos;
	_camera->rotation = origin_rot;
}
