/*
 * rg_demo.cpp
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#include "rg_demo.h"
#include "rg_loader.h"
#include "rg_vecmath.h"
#include "rg_level.h"

static rg_demo* demo;

static rg_demo_frame current_frame;
static double demo_time;
static bool is_demo = false;

void rg_demo_update(double dt) {
	if(demo == NULL)
		return;

	demo_time += dt;
	if(demo_time > demo->duration) {
		SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Demo ended!");
		rg_demo_unload();
		return;
	}

	int f0 = 0;
	for (size_t index = 0; index < demo->keyframes - 1; ++index) {
		if (demo_time < demo->frames[index + 1].time_stamp) {
			f0 = index;
			break;
		}
	}
	int f1 = f0 + 1;

	rg_demo_frame frame0 = demo->frames[f0];
	rg_demo_frame frame1 = demo->frames[f1];

	double midWayLength = demo_time - frame0.time_stamp;
	double framesDiff = frame1.time_stamp - frame0.time_stamp;
	double demo_delta = midWayLength / framesDiff;

	current_frame.time_stamp = demo_time;

	vec3_lerp(&current_frame.position, &frame0.position, &frame1.position, demo_delta);
	vec3_lerp(&current_frame.rotation, &frame0.rotation, &frame1.rotation, demo_delta);
	vec4_lerp(&current_frame.color, &frame0.color, &frame1.color, demo_delta);

}

void rg_demo_load(rg_string path) {
	rg_Resource* res = rg_loadResource(path);
	if(!res)
		return;

	rg_demo_header* header = (rg_demo_header*)res->data;
	demo = (rg_demo*)rg_malloc(sizeof(rg_demo));
	demo->frames = (rg_demo_frame*)rg_malloc(sizeof(rg_demo_frame) * header->keyframes);
	demo->keyframes = header->keyframes;
	demo->duration = header->duration;
	demo_time = 0;
	memcpy(demo->frames, res->data + sizeof(rg_demo_header), sizeof(rg_demo_frame) * header->keyframes);
	rg_freeResource(res);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Demo loaded on %s", header->level_name);
	is_demo = true;
}

void rg_demo_unload() {
	rg_demo_free(demo);
	demo = NULL;
	is_demo = false;
}

rg_demo_frame* rg_demo_currentFrame() {
	return &current_frame;
}

bool rg_demo_running() {
	return is_demo;
}

void rg_demo_write(rg_demo* demo, rg_string file) {
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Saving demo t: %lf, kf: %d", demo->duration, demo->keyframes);

	FILE* f = fopen(file, "wb");

	rg_demo_header header;
	header.duration = demo->duration;
	header.magic[0] = 'r'; header.magic[1] = 'd'; header.magic[2] = 'e'; header.magic[3] = 'm';
	header.keyframes = demo->keyframes;
	memset(header.level_name, '\0', 128);
	strcpy(header.level_name, rg_level->levelname);

	fwrite(&header, sizeof(rg_demo_header), 1, f);
	fwrite(demo->frames, sizeof(rg_demo_frame), demo->keyframes, f);

	fclose(f);
}

void rg_demo_free(rg_demo* demo) {
	rg_free(demo->frames);
	rg_free(demo);
}
