/*
 * cl_texture.h
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_TEXTURE_H_
#define CL_TEXTURE_H_

#include <rg_engine.h>

typedef Uint32 cl_texture_t;

cl_texture_t cl_getTexture(rg_string path);
void cl_deleteTexture(cl_texture_t texture);

#endif /* CL_TEXTURE_H_ */
