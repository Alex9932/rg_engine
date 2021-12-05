/*
 * rg_loader.h
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#ifndef RG_LOADER_H_
#define RG_LOADER_H_

#include <rg_animation.h>
#include "rg_engine.h"
#include <SDL2/SDL.h>

typedef struct rg_Resource {
	Uint32 length;
	void* data;
} rg_Resource;

typedef struct rg_rmlHeader_t {
	char magic[4];
	Uint32 vertices;
	Uint32 indices;
	//Uint8 rml_version;
	Uint32 rml_version;
} rg_rmlHeader_t;

typedef struct AnimHeader {
	char magic[4]; // anim
	Uint32 bone_count;
	Uint32 frame_count;
	Uint32 tps;
	double duration;
} AnimHeader;

rg_Resource* rg_loadResource(rg_string file);
void rg_freeResource(rg_Resource* res);

Animation* rg_loadAnimation(rg_string name);
Animation* rg_convertAnimation(void* data);
void rg_writeAnimation(rg_string file, Animation* data);
void rg_freeAnimation(Animation* anim);

// rml_version == 2
AnimatedMesh* rg_armlConvert(void* data);
void rg_armlWrite(rg_string file, AnimatedMesh* mesh);

// rml_version == 1
rg_mesh_t* rg_rmlConvert(void* data);
void rg_rmlWrite(rg_string file, rg_mesh_t* mesh);

#endif /* RG_LOADER_H_ */
