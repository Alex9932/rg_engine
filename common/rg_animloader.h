/*
 * rg_animloader.h
 *
 *  Created on: Oct 6, 2021
 *      Author: alex9932
 */

#ifndef RG_ANIMLOADER_H_
#define RG_ANIMLOADER_H_

#include "rg_anim.h"
#include "rg_engine.h"
#include <SDL2/SDL.h>

/*
 * header
 * frame0
 *  bone0
 *  bone1
 *  ...
 * frame1
 *  bone0
 *  bone1
 *  ...
 * ...
 */

typedef struct AnimHeader {
	char magic[4]; // anim
	Uint32 bone_count;
	Uint32 frame_count;
	Uint32 tps;
	double duration;
} AnimHeader;

Animation* rg_convertAnimation(void* data);
void rg_writeAnimation(rg_string file, Animation* data);
void rg_freeAnimation(Animation* anim);

#endif /* RG_ANIMLOADER_H_ */
