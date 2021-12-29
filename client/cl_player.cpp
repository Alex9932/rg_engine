/*
 * cl_player.cpp
 *
 *  Created on: Sep 14, 2021
 *      Author: alex9932
 */

#include "cl_player.h"
#include "cl_input.h"
#include <rg_engine.h>
#include <rg_physics.h>

static Camera* _camera;
static vec3 delta;
static float _rotation_x = 0;

static const float S_SPEED = 0.3;
static const float D_SPEED = 3;
static const float B_SPEED = 30;

//static const float MAX_VEL = 1.2;

//static const float HEIGHT = 1.65f;
//static const float GRAVITY = 0.581f;

//static const float GROUND = 2.0f;

//static bool onGround = false;

//static const float S_SPEED = 0.3;
//static const float D_SPEED = 3;
//static const float B_SPEED = 30;
static float SPEED = 0.03;

static float LENGTH = 1.6;

static rg_phys_player* player_object;
static float x = 0;
static float y = 3;
static float z = 0;

static bool _cl_event(rg_Event* event) {

	return true;
}

void cl_player_init(Camera* cam) {
	rg_registerEventHandler(_cl_event);
	_camera = cam;
	player_object = rg_phys_getPlayerObject();
	dBodySetPosition(player_object->body, x, y, z);
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
	vec3_normalize(&fwd, &fwd);

	vec3_cross(&left, &up_y, &fwd);

	vec3_normalize(&up, &up);
	vec3_normalize(&fwd, &fwd);
	vec3_normalize(&left, &left);

	vec3_mul(&up, &up, (float)(dt * SPEED));
	vec3_mul(&fwd, &fwd, (float)(dt * SPEED));
	vec3_mul(&left, &left, (float)(dt * SPEED));

//	if(!onGround) {
//		delta.y -= GRAVITY * dt;
//
//		vec3_mul(&up, &up, 0.025f);
//		vec3_mul(&fwd, &fwd, 0.025f);
//		vec3_mul(&left, &left, 0.025f);
//	}

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

//	x += delta.x;
//	y += delta.y;
//	z += delta.z;


	dMatrix3 mat = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0
	};

	mat4 matrix;
	mat4_identity(&matrix);
	mat4_rotate(&matrix, 3.141592/2, 0, 0);

	mat[0]  = matrix.m00;
	mat[1]  = matrix.m01;
	mat[2]  = matrix.m02;
	mat[3]  = 0;
	mat[4]  = matrix.m10;
	mat[5]  = matrix.m11;
	mat[6]  = matrix.m12;
	mat[7]  = 0;
	mat[8]  = matrix.m20;
	mat[9]  = matrix.m21;
	mat[10] = matrix.m22;
	mat[11] = 0;

	dBodySetRotation(player_object->body, mat);
	//dGeomSetRotation(player_object->geom, mat);

	const dReal* _v = dBodyGetLinearVel(player_object->body);
	dReal v[3];
	v[0] = _v[0];
	v[1] = _v[1];
	v[2] = _v[2];
//	SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Linear vel: %f %f %f", v[0], v[1], v[2]);


	if(cl_isKeyPressed(SDL_SCANCODE_SPACE) && v[1] < 0.1) {
		delta.y = 0.2;
	}

	float k = 0.99999999;
	float _x = v[0] * k;// * dt;
//	float _y = v[1] * k;// * dt;
	float _z = v[2] * k;// * dt;

	v[0] -= _x;
//	v[1] -= _y;
	v[2] -= _z;

	dBodySetLinearVel(player_object->body, v[0], v[1], v[2]);

//	vec3 vel;
//	vel.x = v[0];
//	vel.y = v[1];
//	vel.z = v[2];
//	float len = vec3_length(&vel);
//	if(len < MAX_VEL) {
		dBodyAddForce(player_object->body, delta.x * 50010, delta.y * 50010, delta.z * 50010);
//	}

	delta.x = 0;
	delta.y = 0;
	delta.z = 0;

//	rg_phys_applyForce(player_object, &delta);

//	dGeomSetPosition(player_object->geom, x, y, z);
//	dBodySetPosition(player_object->body, x, y, z);



	vec3 p_pos;
	rg_phys_getPosition(&p_pos, player_object->body);
	_camera->position.x = p_pos.x;
	_camera->position.y = p_pos.y + LENGTH;
	_camera->position.z = p_pos.z;

//	if(onGround) {
//		delta.x = 0;
//		delta.z = 0;
//	}
//
//	onGround = false;
//	if(_camera->position.y <= GROUND) {
//		onGround = true;
//		delta.y = 0;
//		_camera->position.y = GROUND;
//	}

//	SDL_Log("Camera at: %f %f %f %d", _camera->position.x, _camera->position.y, _camera->position.z, onGround);
}
