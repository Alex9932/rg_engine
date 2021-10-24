/*
 * cl_sound.h
 *
 *  Created on: Aug 28, 2021
 *      Author: alex9932
 */

#ifndef CL_SOUND_H_
#define CL_SOUND_H_

#include <AL/al.h>
#include <AL/alc.h>
#include <rg_engine.h>

typedef ALuint Sound;
typedef struct SoundSource {
	ALuint source;
	Sound sound;
	float volume;
	float x;
	float y;
	float z;
} SoundSource;

// --------------------------------------- //
void cl_snd_init();
void cl_snd_quit();
void cl_snd_setMasterVolume(float v);
float cl_snd_getMasterVolume();

// --------------------------------------- //
void cl_snd_setLisPos(float x, float y, float z);
//void cl_snd_setLisPos(float dx, float dy, float dz);

// --------------------------------------- //
SoundSource* cl_snd_newSource();
void cl_snd_deleteSource(SoundSource* src);
void cl_snd_setSourcePos(SoundSource* src, float x, float y, float z);
void cl_snd_setSourceSound(SoundSource* src, Sound snd);
void cl_snd_setSourceVolume(SoundSource* src, float vol);
void cl_snd_play(SoundSource* src);
void cl_snd_stop(SoundSource* src);
void cl_snd_pause(SoundSource* src);
bool cl_snd_isPlaying(SoundSource* src);
void cl_snd_repeat(SoundSource* src, bool r);

// --------------------------------------- //
Sound cl_snd_newSound(rg_string path);
void cl_snd_deleteSound(Sound snd);

#endif /* CL_SOUND_H_ */
