/*
 * cl_main.cpp
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: alex9932
 */

#include "client.h"
#include <SDL2/SDL.h>
#include <rg_engine.h>
#include <rg_level.h>
#include <rg_loader.h>
#include <rg_demo.h>

#include "cl_display.h"
#include "cl_input.h"
#include "cl_renderer.h"
#include "cl_material.h"
#include "cl_gui.h"
#include "cl_sound.h"
#include "cl_freecam.h"
#include "cl_player.h"
#include "cl_democam.h"

char* LOADING_STATUS;

static bool _load_materials = false;
static bool _unloadlevel = false;
static bool _cl_isFreecam = true;

//static vec4 _player;

static bool _cl_event_handler(rg_Event* event) {
	switch (event->type) {
		case RG_EVENT_SHUTDOWN: {
			break;
		}
		case RG_EVENT_LOADLEVEL: {
			_load_materials = true;
			break;
		}
		case RG_EVENT_SDL: {
			if(event->event.type == SDL_KEYDOWN) {
				switch (event->event.key.keysym.scancode) {
					case SDL_SCANCODE_F11:
						cl_display_toggleFullscreen();
						break;
					case SDL_SCANCODE_F1:
						_unloadlevel = true;
						break;
					case SDL_SCANCODE_C:
						_cl_isFreecam = !_cl_isFreecam;
						break;
					case SDL_SCANCODE_F2:
						if(rg_level == NULL) {
							rg_level = rg_newLevel();
							if(rg_loadLevel(rg_level, "rg_test_") == -1) {
								rg_freeLevel(rg_level);
								rg_level = NULL;
							}
						}
						break;
					case SDL_SCANCODE_F12:
						if(rg_level == NULL) {
							rg_level = rg_newLevel();
//							if(rg_loadLevel(rg_level, "rg_sponza") == -1) {
							if(rg_loadLevel(rg_level, "rg_physics") == -1) {
//							if(rg_loadLevel(rg_level, "rg_test_") == -1) {
//							if(rg_loadLevel(rg_level, "rg_test") == -1) {
								rg_freeLevel(rg_level);
								rg_level = NULL;
							}
						}
						break;
					case SDL_SCANCODE_F3:
						rg_demo_load("record.rdem");
						cl_democam_start();
						break;
					case SDL_SCANCODE_R:
						cl_freecam_addFrame();
						break;
					case SDL_SCANCODE_F10:
						cl_freecam_recordDemo();
						break;
					case SDL_SCANCODE_F9:
						cl_freecam_stopDemo();
						rg_demo* demo = cl_freecam_getDemo();
						rg_demo_write(demo, "record.rdem");
						rg_demo_free(demo);
						break;
				}
			}

			if(event->event.type == SDL_WINDOWEVENT) {
				switch (event->event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Size changed: %dx%d", (int)cl_display_getWidth(), (int)cl_display_getHeight());
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						if(rg_isDebug) { SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "FOCUS LOST"); }
						cl_display_setSwapInterval(2);
						break;
					case SDL_WINDOWEVENT_TAKE_FOCUS:
						if(rg_isDebug) { SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "FOCUS TAKED"); }
						cl_display_setSwapInterval(0);
						break;
					default:
						break;
				}
			}
			break;
		}
		default:
			break;
	}

	return true;
}

static SoundSource* src;

void cl_main() {
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Initializing client");

	cl_display_init();
	cl_inputInit();
	rg_registerEventHandler(_cl_event_handler);
	cl_display_open();
	cl_r_init();
	cl_guiInit();
	cl_freecam_init(cl_r_getCamera());
	cl_player_init(cl_r_getCamera());
	cl_democam_init(cl_r_getCamera());

	cl_snd_init();

	src = cl_snd_newSource();
	Sound snd = cl_snd_newSound("gamedata/bg.ogg");
	cl_snd_setSourceSound(src, snd);
	cl_snd_setSourceVolume(src, 0.022f);
//	cl_snd_play(src);
}

void _cl_unload_level() {
	if(rg_level != NULL) {
		cl_r_canRender(false);
		SDL_Log("Unloading level");
		cl_r_unloadMeshes();
		cl_mat_unloadAll();
		rg_freeLevel(rg_level);
		rg_level = NULL;
	}
}

void cl_quit() {
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Client: shutdown");
	_cl_unload_level();
	cl_r_destroy();
	cl_snd_quit();
	cl_display_close();
}

static Uint32 gi = 0;
static Uint32 ti = 0;
static bool tex = false;
static bool do_load = false;

static void cl_prepareLoadQueue() {
	gi = 0;
	ti = 0;
	tex = false;
	do_load = true;
}

static float a = 0;

void cl_update(double dt) {

	a += 2 * dt;
	cl_snd_setSourcePos(src, (float)cos(a), 0, (float)sin(a));

	if(_load_materials) {
		cl_gui_showLoadingScreen();
		cl_r_canRender(false);
		_load_materials = false;
		cl_prepareLoadQueue();
	}

	if(do_load) {
		if(tex) { // Load textures
			if(ti == 0) {
				SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Loading textures");
				LOADING_STATUS = (char*)"Loading textures";
				cl_mat_unloadAll();
			}
			cl_mat_load(rg_level->materials[ti]);
			ti++;
			if(ti >= rg_level->materials.size()) {
				do_load = false;
				// Loading done!
				cl_r_canRender(true);
				cl_gui_hide();
			}
		} else { // Load geometry
			if(gi == 0) {
				SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Loading geometry");
				LOADING_STATUS = (char*)"Loading geometry";
				cl_r_unloadMeshes();
			}
			cl_r_loadMesh(rg_level->meshes[gi]);
			gi++;
			if(gi >= rg_level->meshes.size()) {
				tex = true;
			}
		}
	}

	cl_inputUpdate();

	if(rg_demo_running()) {
		rg_demo_update(dt);
		cl_democam_update();
	} else {
		if(_cl_isFreecam) {
			cl_freecam_update(dt);
		} else {
			cl_player_update(dt);
		}
	}

//	_player.x = cl_r_getCamera()->position.x;
//	_player.y = cl_r_getCamera()->position.y;
//	_player.z = cl_r_getCamera()->position.z;
//	_player.w = cl_r_getCamera()->rotation.x;


	cl_r_doRender(dt);
	cl_display_update();

	if(_unloadlevel) {
		_unloadlevel = false;
		_cl_unload_level();
	}
}
