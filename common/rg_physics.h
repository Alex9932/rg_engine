/*
 * rg_physics.h
 *
 *  Created on: Nov 6, 2021
 *      Author: alex9932
 */

#ifndef RG_PHYSICS_H_
#define RG_PHYSICS_H_

#include <ode/ode.h>
#include "rg_vecmath.h"

#define RG_ODE_AXIS_X 1
#define RG_ODE_AXIS_Y 2
#define RG_ODE_AXIS_Z 3

typedef struct rg_phys_object {
	dBodyID body;
	dGeomID geom;
} rg_phys_object;

typedef struct rg_phys_player {
	dBodyID body;
	dGeomID geom;
	bool onGround;

} rg_phys_player;

void rg_phys_init();
void rg_phys_destroy();
void rg_phys_update(double delta);
void rg_phys_clearWorld();

void rg_phys_applyForce(rg_phys_object* obj, vec3* f_vec);

rg_phys_player* rg_phys_getPlayerObject();

rg_phys_object* rg_phys_createBox(float x, float y, float z, float w, float h, float d, float mass);
rg_phys_object* rg_phys_createSphere(float x, float y, float z, float r, float mass);

void rg_phys_freeBody(rg_phys_object* body);

void rg_phys_getMatrix(mat4* matrix, dBodyID body);
void rg_phys_getPosition(vec3* pos, dBodyID body);

#endif /* RG_PHYSICS_H_ */
