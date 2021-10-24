/*
 * rg_animloader.cpp
 *
 *  Created on: Oct 6, 2021
 *      Author: alex9932
 */

#include "rg_animloader.h"
#include <stdio.h>

Animation* rg_convertAnimation(void* data) {
	AnimHeader* header = (AnimHeader*)data;
	data += sizeof(AnimHeader);

	Animation* anim = (Animation*)rg_malloc(sizeof(Animation));

	anim->bone_count = header->bone_count;
	anim->frame_count = header->frame_count;
	anim->tps = header->tps;
	anim->duration = header->duration;

	anim->frames = (KeyFrame*)rg_malloc(sizeof(KeyFrame) * anim->frame_count);
	for (size_t i = 0; i < anim->frame_count; ++i) {
		anim->frames[i].bones = (BoneTransform*)rg_malloc(sizeof(BoneTransform) * anim->bone_count);
		anim->frames[i].time_stamp = *(double*)data;
		data += sizeof(double);
		for (size_t j = 0; j < anim->bone_count; ++j) {
			BoneTransform* bt = (BoneTransform*)data;
			data += sizeof(BoneTransform);
			anim->frames[i].bones[j].id = bt->id;
			anim->frames[i].bones[j].position = bt->position;
			anim->frames[i].bones[j].scale = bt->scale;
			anim->frames[i].bones[j].orientation = bt->orientation;
		}
	}

	return anim;
}

void rg_writeAnimation(rg_string file, Animation* data) {
	FILE* f = fopen(file, "wb");
	AnimHeader header;
	header.magic[0] = 'a'; header.magic[1] = 'n'; header.magic[2] = 'i'; header.magic[3] = 'm';
	header.bone_count = data->bone_count;
	header.frame_count = data->frame_count;
	header.tps = data->tps;
	header.duration = data->duration;
	fwrite(&header, sizeof(AnimHeader), 1, f);

	for (size_t i = 0; i < data->frame_count; ++i) {
		fwrite(&data->frames[i].time_stamp, sizeof(double), 1, f);
		for (size_t j = 0; j < data->bone_count; ++j) {
			fwrite(&data->frames[i].bones[j], sizeof(BoneTransform), 1, f);
		}
	}

	fclose(f);
}

void rg_freeAnimation(Animation* anim) {
	for (size_t i = 0; i < anim->frame_count; ++i) {
		rg_free(anim->frames[i].bones);
	}
	rg_free(anim->frames);
	rg_free(anim);
}
