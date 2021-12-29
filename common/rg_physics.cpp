/*
 * rg_physics.cpp
 *
 *  Created on: Nov 6, 2021
 *      Author: alex9932
 */

#include "rg_physics.h"
#include <SDL2/SDL.h>
#include <vector>

#define MAX_CONTACTS 32
#define SIM_STEP_MULTIPLIER 1

static dWorldID      d_world;
static dSpaceID      d_space;
static dGeomID       d_ground;
static dJointGroupID d_contactgroup;

static std::vector<rg_phys_object*> phys_bodys;

static rg_phys_player* player = NULL;

static void _rg_phys_callback(void* data, dGeomID o1, dGeomID o2) {
	double mu0 = 100;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contact[MAX_CONTACTS];

	bool is_player = false;
	if(player && (player->geom == o1 || player->geom == o2)) {
		mu0 = 0;
		is_player = true;
	}

	int n = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
	int i;
	for (i = 0; i < MAX_CONTACTS; i++) {
		contact[i].surface.mode = dContactSoftERP;
		if(!is_player) {
			contact[i].surface.mode |= dContactSoftCFM | dContactBounce;
		}
		contact[i].surface.mu = mu0;
//		contact[i].surface.bounce = 0.401;
//		contact[i].surface.bounce_vel = 0.3;
		contact[i].surface.bounce = 0.0;
		contact[i].surface.bounce_vel = 0.0;
		contact[i].surface.soft_cfm = 0.0;
		contact[i].surface.soft_erp = 0.01;
	}

	for (i = 0; i < n; i++) {
		dJointID c = dJointCreateContact(d_world, d_contactgroup, contact + i);
		dJointAttach(c, b1, b2);

		if(is_player) {
			player->onGround = true;
		}
	}
}

static rg_phys_object* rg_phys_makeBody() {
	rg_phys_object* dbody = (rg_phys_object*)rg_malloc(sizeof(rg_phys_object));
	phys_bodys.push_back(dbody);
	return dbody;
}

void rg_phys_init() {
	SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Init world");
	dInitODE2(0);
	d_world = dWorldCreate();
	d_space = dSimpleSpaceCreate(0);

	dWorldSetGravity(d_world, 0, -9.81, 0);
	dWorldSetERP(d_world, 0.2);
	dWorldSetCFM(d_world, 1e-5);
	d_contactgroup = dJointGroupCreate(0);
	d_ground = dCreatePlane(d_space, 0, 1, 0, 0);
}

void rg_phys_destroy() {
	SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Destroy world");
	if(player) {
		dGeomDestroy(player->geom);
		dBodyDestroy(player->body);
		rg_free(player);
	}
	dJointGroupDestroy(d_contactgroup);
	dSpaceDestroy(d_space);
	dWorldDestroy(d_world);
	dCloseODE();
}

void rg_phys_update(double delta) {
	if(delta <= 0 || delta > 1) {
		SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Lag detected! Skipping...");
		return;
	}

//	int nj = dBodyGetNumJoints(player->body);
//	SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Joints: %d", nj);

	dSpaceCollide(d_space, (void*)NULL, &_rg_phys_callback);
	dWorldQuickStep(d_world, delta * SIM_STEP_MULTIPLIER);
	dJointGroupEmpty(d_contactgroup);
}

void rg_phys_clearWorld() {
	SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Clearing world");
	for(unsigned int i = 0; i < phys_bodys.size(); i++) {
		rg_phys_freeBody(phys_bodys[i]);
	}

	phys_bodys.clear();
}

void rg_phys_applyForce(rg_phys_object* obj, vec3* f_vec) {
	dBodyAddForce(obj->body, f_vec->x, f_vec->y, f_vec->z);
}

rg_phys_player* rg_phys_getPlayerObject() {
	if(player == NULL) {
		player = (rg_phys_player*)rg_malloc(sizeof(rg_phys_player));

//		player->geom = dCreateCylinder(d_space, 0.9, 1.75);
		player->geom = dCreateCapsule(d_space, 0.9, 1.75);
		player->body = dBodyCreate(d_world);

		dMass _mass;
//		dMassSetCylinder(&_mass, 5, RG_ODE_AXIS_Y, 0.9, 1.75);
		dMassSetCapsule(&_mass, 5, RG_ODE_AXIS_Y, 0.9, 1.75);
		dBodySetMass(player->body, &_mass);
		dGeomSetBody(player->geom, player->body);
	}
	return player;
}

rg_phys_object* rg_phys_createBox(float x, float y, float z, float w, float h, float d, float mass) {
	rg_phys_object* body = rg_phys_makeBody();
	body->geom = dCreateBox(d_space, w, h, d);
	body->body = dBodyCreate(d_world);
	dMass _mass;
	dMassSetBox(&_mass, mass, w, h, d);
	dBodySetMass(body->body, &_mass);
	dGeomSetBody(body->geom, body->body);
	dBodySetPosition(body->body, x, y, z);
	return body;
}

rg_phys_object* rg_phys_createSphere(float x, float y, float z, float r, float mass) {
	rg_phys_object* body = rg_phys_makeBody();
	body->geom = dCreateSphere(d_space, r);
	body->body = dBodyCreate(d_world);
	dMass _mass;
	dMassSetSphere(&_mass, mass, r);
	dBodySetMass(body->body, &_mass);
	dGeomSetBody(body->geom, body->body);
	dBodySetPosition(body->body, x, y, z);
	return body;
}

void rg_phys_freeBody(rg_phys_object* body) {
	dGeomDestroy(body->geom);
	dBodyDestroy(body->body);
	rg_free(body);
}

void rg_phys_getMatrix(mat4* matrix, dBodyID body) {
	const double* pos = dBodyGetPosition(body);
	const double* rot = dBodyGetRotation(body);
	float* mat = (float*)matrix;
	mat[0]  = rot[0]; mat[1]  = rot[4]; mat[2]  = rot[8];  mat[3]  = 0;
	mat[4]  = rot[1]; mat[5]  = rot[5]; mat[6]  = rot[9];  mat[7]  = 0;
	mat[8]  = rot[2]; mat[9]  = rot[6]; mat[10] = rot[10]; mat[11] = 0;
	mat[12] = pos[0]; mat[13] = pos[1]; mat[14] = pos[2];  mat[15] = 1;
}

void rg_phys_getPosition(vec3* pos, dBodyID body) {
	const double* dpos = dBodyGetPosition(body);
	pos->x = dpos[0];
	pos->y = dpos[1];
	pos->z = dpos[2];
}
