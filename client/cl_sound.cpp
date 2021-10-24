/*
 * cl_sound.cpp
 *
 *  Created on: Aug 28, 2021
 *      Author: alex9932
 */

#include "cl_sound.h"

#include <vector>
#include <algorithm>
#include <rg_engine.h>
#include <stb/stb_vorbis.h>


#define LOAD_FROM_RG_RESOURCE


static ALCdevice* device;
static ALCcontext* context;

static std::vector<SoundSource*> sources;
static std::vector<Sound> buffers;

static float master_volume = 1.0f;
static float lx;
static float ly;
static float lz;


// !!! DBG ONLY !!!
// !!! DBG ONLY !!!
// !!! DBG ONLY !!!
//static void _sources_info() {
//	SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
//	SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: SOURCES: %d", sources.size());
//	for (Uint32 i = 0; i < sources.size(); ++i) {
//		SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: src %d -> a 0x%x, v %f, x %f, y %f, z %f", i, sources[i], sources[i]->volume, sources[i]->x, sources[i]->y, sources[i]->z);
//	}
//}

void cl_snd_init() {
	SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: Open device...");
	device = alcOpenDevice(0);
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "AL: Init failed!");
	}
//	_sources_info();
}

void cl_snd_quit() {
	SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: Quit...");
	alcDestroyContext(context);
	alcCloseDevice(device);
//	_sources_info();
}

void cl_snd_setMasterVolume(float v) {
	master_volume = v;
	for (Uint32 i = 0; i < sources.size(); ++i) {
		alSourcef(sources[i]->source, AL_GAIN, sources[i]->volume * master_volume);
	}
}

float cl_snd_getMasterVolume() {
	return master_volume;
}

void cl_snd_setLisPos(float x, float y, float z) {
	lx = x;
	ly = y;
	lz = z;
	alListener3f(AL_POSITION, lx, ly, lz);
}

SoundSource* cl_snd_newSource() {
	SoundSource* src = (SoundSource*)rg_malloc(sizeof(SoundSource));
	alGenSources(1, &src->source);
	src->sound = 0;
	src->volume = 1;
	src->x = 0;
	src->y = 0;
	src->z = 0;
	sources.push_back(src);
//	_sources_info();
	return src;
}

void cl_snd_deleteSource(SoundSource* src) {
	alSourceStop(src->source);
	alDeleteSources(1, &src->source);
	sources.erase(std::remove(sources.begin(), sources.end(), src), sources.end());
	rg_free(src);
//	_sources_info();
}

void cl_snd_setSourcePos(SoundSource* src, float x, float y, float z) {
	src->x = x;
	src->y = y;
	src->z = z;
	alSource3f(src->source, AL_POSITION, src->x, src->y, src->z);
}

void cl_snd_setSourceSound(SoundSource* src, Sound snd) {
	src->sound = snd;
	alSourcei(src->source, AL_BUFFER, snd);
	alSourcef(src->source, AL_ROLLOFF_FACTOR, 1.5f);
}

void cl_snd_setSourceVolume(SoundSource* src, float vol) {
	src->volume = vol;
	alSourcef(src->sound, AL_GAIN, vol);
}

void cl_snd_play(SoundSource* src) {
	alSourcePlay(src->source);
}

void cl_snd_stop(SoundSource* src) {
	alSourceStop(src->source);
}

void cl_snd_pause(SoundSource* src) {
	alSourcePause(src->source);
}

bool cl_snd_isPlaying(SoundSource* src) {
	ALint state;
	alGetSourcei(src->source, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}

void cl_snd_repeat(SoundSource* src, bool r) {
	alSourcei(src->source, AL_LOOPING, r ? AL_TRUE : AL_FALSE);
}

Sound cl_snd_newSound(rg_string path) {
	Sound buffer;
	alGenBuffers(1, &buffer);
	short* data;
	int sample_rate;
	int channels;
	SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "AL: Loading sound: %s", path);

#ifdef LOAD_FROM_RG_RESOURCE
	rg_Resource* res = rg_loadResource(path);
	int rc = stb_vorbis_decode_memory((const unsigned char*)res->data, res->length, &channels, &sample_rate, &data);
#else
	int rc = stb_vorbis_decode_filename(path, &channels, &sample_rate, &data);
#endif
	if (rc == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "AL: Unable to load file!");
	}
	ALenum format;
	if (channels == 1) {
		format = AL_FORMAT_MONO16;
	} else {
		format = AL_FORMAT_STEREO16;
	}
	alBufferData(buffer, format, data, rc * channels * sizeof(short), sample_rate);
	ALenum error = alGetError();
	if (error != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "AL: Error code: %d", error);
	}
	buffers.push_back(buffer);
#ifdef LOAD_FROM_RG_RESOURCE
	rg_freeResource(res);
#endif
	return buffer;
}

void cl_snd_deleteSound(Sound snd) {
	alDeleteBuffers(1, &snd);
	buffers.erase(std::remove(buffers.begin(), buffers.end(), snd), buffers.end());
}
