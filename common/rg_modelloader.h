/*
 * rg_modelloader.h
 *
 *  Created on: 13 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_MODELLOADER_H_
#define RG_MODELLOADER_H_

#include <SDL2/SDL.h>
#include "rg_mesh.h"

typedef struct rg_rmlHeader_t {
	char magic[4];
	Uint32 vertices;
	Uint32 indices;
	//Uint8 rml_version;
	Uint32 rml_version;
} rg_rmlHeader_t;

rg_mesh_t* rg_rmlConvert(void* data);
void rg_rmlWrite(rg_string file, rg_mesh_t* mesh);

#endif /* RG_MODELLOADER_H_ */
