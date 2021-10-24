/*
 * cl_player.cpp
 *
 *  Created on: Sep 14, 2021
 *      Author: alex9932
 */

#include "cl_player.h"
#include "cl_input.h"
#include <rg_engine.h>

static cl_camera* _camera;
static vec3 delta;
static float _rotation_x = 0;

static const float S_SPEED = 0.3;
static const float D_SPEED = 3;
static const float B_SPEED = 30;

//static const float HEIGHT = 1.65f;
static const float GRAVITY = 0.581f;

static const float GROUND = 2.0f;

static bool onGround = false;

//static const float S_SPEED = 0.3;
//static const float D_SPEED = 3;
//static const float B_SPEED = 30;
static float SPEED = 0.03;

static bool _cl_event(rg_Event* event) {

	return true;
}

void cl_player_init(cl_camera* cam) {
	rg_registerEventHandler(_cl_event);
	_camera = cam;
}

void cl_player_update(double dt) {
	_camera->rotation.x += cl_getMouseDX();
	_camera->rotation.y += cl_getMouseDY();

	_rotation_x = _camera->rotation.x;


	vec3 up_y = {0, 1, 0};
	vec3 up;
	vec3 fwd;
	vec3 left;
	vec3 neg;

	math_getForward(&fwd, (vec2*)&_camera->rotation);
	fwd.y = 0;

	vec3_cross(&left, &up_y, &fwd);

	vec3_normalize(&up, &up);
	vec3_normalize(&fwd, &fwd);
	vec3_normalize(&left, &left);

	vec3_mul(&up, &up, (float)(dt * SPEED));
	vec3_mul(&fwd, &fwd, (float)(dt * SPEED));
	vec3_mul(&left, &left, (float)(dt * SPEED));

	if(!onGround) {
		delta.y -= GRAVITY * dt;

		vec3_mul(&up, &up, 0.025f);
		vec3_mul(&fwd, &fwd, 0.025f);
		vec3_mul(&left, &left, 0.025f);
	}

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

	if(cl_isKeyPressed(SDL_SCANCODE_SPACE) && onGround) {
		delta.y = 0.1f;
	}

	vec3_add(&_camera->position, &_camera->position, &delta);
//	vec3_mul(&delta, &delta, 0.0f);
	if(onGround) {
		delta.x = 0;
		delta.z = 0;
	}

	onGround = false;
	if(_camera->position.y <= GROUND) {
		onGround = true;
		delta.y = 0;
		_camera->position.y = GROUND;
	}

//	SDL_Log("Camera at: %f %f %f %d", _camera->position.x, _camera->position.y, _camera->position.z, onGround);
}
