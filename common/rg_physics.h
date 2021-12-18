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

typedef struct rg_phys_object {
	dBodyID body;
	dGeomID geom;
} rg_phys_object;

void rg_phys_init();
void rg_phys_destroy();
void rg_phys_update(double delta);
void rg_phys_clearWorld();

rg_phys_object* rg_phys_createBox(float x, float y, float z, float w, float h, float d, float mass);
rg_phys_object* rg_phys_createSphere(float x, float y, float z, float r, float mass);

void rg_phys_freeBody(rg_phys_object* body);

void rg_phys_getMatrix(mat4* matrix, rg_phys_object* body);
void rg_phys_getPosition(vec3* pos, rg_phys_object* body);

#endif /* RG_PHYSICS_H_ */
