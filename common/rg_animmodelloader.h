/*
 * rg_animmodelloader.h
 *
 *  Created on: Oct 5, 2021
 *      Author: alex9932
 */

#ifndef RG_ANIMMODELLOADER_H_
#define RG_ANIMMODELLOADER_H_

#include <SDL2/SDL.h>
#include "rg_modelloader.h"
#include "rg_anim.h"
#include "rg_engine.h"

// rml_version == 2

AnimatedMesh* rg_armlConvert(void* data);
void rg_armlWrite(rg_string file, AnimatedMesh* mesh);

#endif /* RG_ANIMMODELLOADER_H_ */
