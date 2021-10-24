/*
 * cl_particle.h
 *
 *  Created on: 9 мая 2021 г.
 *      Author: alex9932
 */

#ifndef CL_PARTICLE_H_
#define CL_PARTICLE_H_

#include <rg_vecmath.h>

typedef struct cl_particle {
//	vec3 position;
//	vec3 str; // size, time, rotation
//	vec3 velocity;
	float x;
	float y;
	float z;
	float s;
	float t;
	float r;
	float vx;
	float vy;
	float vz;
} cl_particle;


#endif /* CL_PARTICLE_H_ */
