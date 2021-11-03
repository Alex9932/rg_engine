/*
 * rg_demo.h
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#ifndef RG_DEMO_H_
#define RG_DEMO_H_

#include "rg_engine.h"
#include "rg_vecmath.h"

typedef struct rg_demo_frame {
	double time_stamp;
	// Camera orientation
	vec3 position;
	vec3 rotation;

	vec4 color; // For fade in/out effects
} rg_demo_frame;

typedef struct rg_demo {
	double duration;
	Uint32 keyframes;
	rg_demo_frame* frames;
} rg_demo;

// Demo file only
typedef struct rg_demo_header {
	char magic[4]; // rdem
	Uint32 keyframes;
	double duration;
	char level_name[128];
} rg_demo_header;

void rg_demo_update(double dt);
void rg_demo_load(rg_string path);
void rg_demo_unload();
rg_demo_frame* rg_demo_currentFrame();
bool rg_demo_running();

void rg_demo_write(rg_demo* demo, rg_string file);
void rg_demo_free(rg_demo* demo);

#endif /* RG_DEMO_H_ */
