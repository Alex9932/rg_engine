/*
 * cl_material.h
 *
 *  Created on: 28 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_MATERIAL_H_
#define CL_MATERIAL_H_

#include <rg_vecmath.h>
#include <rg_level.h>
#include "cl_texture.h"
#include <SDL2/SDL.h>

typedef struct rg_material_t {
	Uint32 mat_id;
	cl_texture_t diffuse;
	cl_texture_t normal;
	cl_texture_t roughness;
	cl_texture_t metallic;
	cl_texture_t glow;
} rg_material_t;

void cl_mat_load(rg_string name);
void cl_mat_unloadAll();
rg_material_t* cl_mat_get(Uint32 mat_id);

#endif /* CL_MATERIAL_H_ */
