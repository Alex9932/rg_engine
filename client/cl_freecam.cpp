/*
 * cl_freecam.cpp
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_freecam.h"
#include "cl_input.h"
#include <rg_engine.h>
#include <rg_vecmath.h>

#include <vector>

static cl_camera* _camera;
static vec3 delta;

static bool demo_record = false;
static double demo_time = 0;
static std::vector<rg_demo_frame> demo_frames;

static const float S_SPEED = 0.3;
static const float D_SPEED = 3;
static const float B_SPEED = 30;
//static const float S_SPEED = 0.3;
//static const float D_SPEED = 3;
//static const float B_SPEED = 30;
static float SPEED = 0.03;

static bool _cl_event(rg_Event* event) {

	return true;
}

void cl_freecam_init(cl_camera* cam) {
	rg_registerEventHandler(_cl_event);
	_camera = cam;
}

//static void _cl_tomat3(mat3* dest, mat4* src) {
//	dest->m00 = src->m00; dest->m01 = src->m01; dest->m02 = src->m02;
//	dest->m10 = src->m10; dest->m11 = src->m11; dest->m12 = src->m12;
//	dest->m20 = src->m20; dest->m21 = src->m21; dest->m22 = src->m22;
//}

void cl_freecam_update(double dt) {
	if(demo_record)
		demo_time += dt;

	_camera->rotation.x += cl_getMouseDX();
	_camera->rotation.y += cl_getMouseDY();

	if(cl_isKeyPressed(SDL_SCANCODE_Q)) {
		_camera->rotation.z -= 0.9 * dt;
	}

	if(cl_isKeyPressed(SDL_SCANCODE_E)) {
		_camera->rotation.z += 0.9 * dt;
	}

//	mat3 rot_mat;
//	vec3 fwd_z = {0, 0, 1};
	vec3 up_y = {0, 1, 0};
	vec3 up;
	vec3 fwd;
	vec3 left;
	vec3 neg;

	math_getForward(&fwd, (vec2*)&_camera->rotation);

//	_cl_tomat3(&rot_mat, &_camera->view);
//
//	mat3_mul(&fwd, &fwd_z, &rot_mat);
//	mat3_mul(&up, &up_y, &rot_mat);
//	vec3_cross(&left, &up, &fwd);
	vec3_cross(&left, &up_y, &fwd);

	vec3_normalize(&up, &up);
	vec3_normalize(&fwd, &fwd);
	vec3_normalize(&left, &left);

	vec3_mul(&up, &up, (float)(dt * SPEED));
	vec3_mul(&fwd, &fwd, (float)(dt * SPEED));
	vec3_mul(&left, &left, (float)(dt * SPEED));

	SPEED = D_SPEED;
	if(cl_isKeyPressed(SDL_SCANCODE_LSHIFT)) {
		SPEED = S_SPEED;
	}
	if(cl_isKeyPressed(SDL_SCANCODE_LCTRL)) {
		SPEED = B_SPEED;
	}

	if(cl_isKeyPressed(SDL_SCANCODE_W)) {
		vec3_add(&delta, &delta, &fwd);
	}

	if(cl_isKeyPressed(SDL_SCANCODE_S)) {
		vec3_negate(&neg, &fwd);
		vec3_add(&delta, &delta, &neg);
	}

	if(cl_isKeyPressed(SDL_SCANCODE_A)) {
		vec3_add(&delta, &delta, &left);
	}

	if(cl_isKeyPressed(SDL_SCANCODE_D)) {
		vec3_negate(&neg, &left);
		vec3_add(&delta, &delta, &neg);
	}

	vec3_add(&_camera->position, &_camera->position, &delta);

	vec3_mul(&delta, &delta, 0.0f);

	//SDL_Log("Camera at: %f %f %f", _camera->position.x, _camera->position.y, _camera->position.z);
}

void cl_freecam_recordDemo() {
	demo_frames.clear();
	demo_time = 0;
	demo_record = true;
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Demo recording started!");
}

void cl_freecam_stopDemo() {
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Demo recording ended! Time: %lf", demo_time);
	demo_record = false;
}

void cl_freecam_addFrame() {
	if(!demo_record)
		return;

	rg_demo_frame frame;
	frame.time_stamp = demo_time;
	frame.color = {1, 1, 1, 1};
	frame.position = _camera->position;
	frame.rotation = _camera->rotation;
	demo_frames.push_back(frame);
}

rg_demo* cl_freecam_getDemo() {
	if(demo_record)
		return NULL;

	rg_demo* demo = (rg_demo*)rg_malloc(sizeof(rg_demo));

	demo->duration = demo_time;
	demo->keyframes = demo_frames.size();
	demo->frames = (rg_demo_frame*)rg_malloc(sizeof(rg_demo_frame) * demo->keyframes);

	for (size_t i = 0; i < demo->keyframes; ++i) {
		demo->frames[i] = demo_frames[i];
	}

	return demo;
}
