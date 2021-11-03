/*
 * cl_main.cpp
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: alex9932
 */

#include "client.h"
#include <SDL2/SDL.h>
#include <rg_net.h>
#include <rg_engine.h>
#include <rg_level.h>
#include <rg_string.h>
#include <rg_vecmath.h>

#include "cl_display.h"
#include "cl_input.h"
#include "cl_renderer.h"
#include "cl_material.h"

#include "cl_gui.h"

#include "cl_sound.h"

#include "cl_freecam.h"
#include "cl_player.h"
#include "cl_democam.h"

#include <rg_loader.h>

#include <rg_demo.h>

char* LOADING_STATUS;

//static TCPsocket socket;

//static char _discbyclient[] = "Disconnected by client!";
//static char _GET[] = "GET";

static bool _load_materials = false;
static bool _unloadlevel = false;

static bool _cl_isFreecam = true;

static vec4 _player;

static bool _cl_event_handler(rg_Event* event) {
//	rg_print("cl: Event handled!");

	switch (event->type) {
		case RG_EVENT_SHUTDOWN: {
//			SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Disconnecting...");
//			rg_NetPackage pkg;
//			pkg.pkg_type = RG_NET_PKG_TYPE_DISCONNECT;
//			pkg.length = strlen(_discbyclient);
//			pkg.data = _discbyclient;
//			rg_netSend(socket, &pkg);
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
							if(rg_loadLevel(rg_level, "rg_house") == -1) {
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

static mat4 IDENTITY_MATRIX;

//static int cl_net_handler(void* data) {
//	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "NET: starting");
//	rg_NetPackage s_pkg;
//	Uint32 ip = 0;
//	bool sendPlayer = false;
//
//	while(rg_isRunning()) {
//		ip++;
//
//		if(sendPlayer) {
//			s_pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
//			s_pkg.length = sizeof(vec4);
//			s_pkg.data = &_player;
//			sendPlayer = false;
//		} else {
//			if((ip % 5) == 0) {
//				s_pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
//				s_pkg.length = 6;
//				s_pkg.data = (char*)"PLAYER";
//			} else {
//				s_pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
//				s_pkg.length = SDL_strlen(_GET);
//				s_pkg.data = _GET;
//			}
//		}
//
//		rg_netSend(socket, &s_pkg); // Send request
//
//		rg_NetPackage* pkg = rg_netRecv(socket); // Get response
//		if(pkg->pkg_type == RG_NET_PKG_TYPE_FAILED) {
//			SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Server closed!");
//			rg_stop();
//		}
//
//		if(pkg->pkg_type == RG_NET_PKG_TYPE_DATA) {
//			const char* str = (const char*)pkg->data;
//
//			if(rg_streql(str, "OK")) {
//				sendPlayer = true;
//			}
//		}
//
//		if(pkg->pkg_type == RG_NET_PKG_TYPE_DISCONNECT) {
//			SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Disconnected: %s", (char*)pkg->data);
//			rg_stop();
//		}
//
//		rg_netFreePackage(pkg);
//	}
//	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "NET: quit");
//
//	return 0;
//}

static SoundSource* src;
static Animation* anim;

void cl_main() {
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Initializing client");

	mat4_identity(&IDENTITY_MATRIX);

//	socket = rg_netConnect("localhost", 12345);
//	if(socket) {
//		SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Connected to server!");
//	} else {
//		SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Unable to connect!");
//	}
//	SDL_CreateThread(cl_net_handler, "Net handler", NULL);

	cl_display_init();
	cl_inputInit();
	rg_registerEventHandler(_cl_event_handler);
	cl_display_open();
	cl_r_init();
	cl_freecam_init(cl_r_getCamera());
	cl_player_init(cl_r_getCamera());
	cl_democam_init(cl_r_getCamera());

//	rg_Resource* res = rg_loadResource("gamedata/anims/wolf.anim"); // Load animation
	rg_Resource* res = rg_loadResource("gamedata/anims/vampire.anim"); // Load animation
//	rg_Resource* res = rg_loadResource("gamedata/anims/snake.anim"); // Load animation
	anim = rg_convertAnimation(res->data);
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Loaded animation => tps %d d %lf", anim->tps, anim->duration);
//	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Keyframes:");
//	for (size_t i = 0; i < anim->frame_count; ++i) {
//		SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "frame%d => %lf", i, anim->frames[i].time_stamp);
//	}
	rg_freeResource(res);

	cl_snd_init();

	src = cl_snd_newSource();
	Sound snd = cl_snd_newSound("gamedata/bg.ogg");
	cl_snd_setSourceSound(src, snd);
	cl_snd_setSourceVolume(src, 0.022f);
	cl_snd_play(src);
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
static double animation_time = 0;

static void _cl_calcBoneTransform(rg_object_t* obj, rg_Bone* bone, mat4* parent_transform) {
	mat4 global_transform;
	mat4_mul(&global_transform, parent_transform, &obj->local_transforms[bone->id]); // Mul parent transform by node transform

	mat4 matrix;

	mat4_invert(&matrix, &bone->offset);

//	matrix = bone->offset;

//	printf("~~~~~~~~~~~~\n");
//	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
//		matrix.m00, matrix.m01, matrix.m02, matrix.m03,
//		matrix.m10, matrix.m11, matrix.m12, matrix.m13,
//		matrix.m20, matrix.m21, matrix.m22, matrix.m23,
//		matrix.m30, matrix.m31, matrix.m32, matrix.m33);

//	obj->global_transforms[bone->id] = global_transform;

	for (size_t i = 0; i < bone->child_count; ++i) {
		_cl_calcBoneTransform(obj, &bone->childs[i], &global_transform);
	}

	mat4_mul(&obj->global_transforms[bone->id], &global_transform, &matrix);
//	mat4_mul(&obj->global_transforms[bone->id], &matrix, &global_transform);
}

void cl_update(double dt) {
//	ca += dt;
//
//	float x0 = cb;
//	float x1 = cc;
//	float y0 = prot;
//	float y1 = crot;
//	float xp = ca - cc + cb;
//
//	rot = y0 + ((y1-y0)/(x1-x0)) * (xp - x0);

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
//			SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Try to load: %s", rg_level->meshes[gi]);
			cl_r_loadMesh(rg_level->meshes[gi]);
			gi++;
			if(gi >= rg_level->meshes.size()) {
				tex = true;
			}
		}
	}

	cl_inputUpdate();

//	rot += dt * 5.9;

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

	_player.x = cl_r_getCamera()->position.x;
	_player.y = cl_r_getCamera()->position.y;
	_player.z = cl_r_getCamera()->position.z;
	_player.w = cl_r_getCamera()->rotation.x;


	// Apply animation
	rg_object_t* anim_obj = NULL;
if(rg_level) {
	for (size_t i = 0; i < rg_level->objects.size(); ++i) {
		if(rg_level->objects[i]->type == RG_OBJECT_ANIMATED) {
			anim_obj = rg_level->objects[i];
		}
	}

//	rg_level->objects[0]->rotation.x = SDL_cos(time * 2);
//	rg_level->objects[0]->rotation.z = SDL_sin(time * 1.6);

}
if(anim_obj) {
	animation_time += dt;
	int f0 = 0;
	for (size_t index = 0; index < anim->frame_count - 1; ++index) {
		if (animation_time < anim->frames[index + 1].time_stamp) {
			f0 = index;
			break;
		}
	}
	int f1 = f0 + 1;
	if(animation_time >= anim->frames[f1].time_stamp) {
		animation_time = 0;
	}

	KeyFrame frame0 = anim->frames[f0];
	KeyFrame frame1 = anim->frames[f1];
	double midWayLength = animation_time - frame0.time_stamp;
	double framesDiff = frame1.time_stamp - frame0.time_stamp;
	double anim_delta = midWayLength / framesDiff;

//	printf("Bonecount %d\n", anim->bone_count);

	for (size_t i = 0; i < anim->bone_count; ++i) {
		BoneTransform transform0 = frame0.bones[i];
		BoneTransform transform1 = frame1.bones[i];
		BoneTransform transform; // final transform

//		printf("%d bid: %d", i, transform0.id);
		if(transform0.id == -1) {
//			printf(" => SKIP\n");
			continue;
		}
//		printf("\n");


//		if(f0 == 12) {
//			printf("Frame12(bone%d) => %f %f %f\n", i, transform0.position.x, transform0.position.y, transform0.position.z);
//		}

		// Interpolate
		vec3_lerp(&transform.scale, &transform0.scale, &transform1.scale, anim_delta);
		vec3_lerp(&transform.position, &transform0.position, &transform1.position, anim_delta);
		quat_slerp(&transform.orientation, &transform0.orientation, &transform1.orientation, anim_delta);

		mat4 translate; mat4_identity(&translate);
		mat4 rotate;    mat4_identity(&rotate);
		mat4 scale;     mat4_identity(&scale, &transform.scale);

		mat4_translate(&translate, transform.position.x, transform.position.y, transform.position.z);
		mat4_quat(&rotate, &transform.orientation);

		mat4_mul(&anim_obj->local_transforms[transform0.id], &translate, &rotate);



//		mat4 matrix = translate;
//		printf("~~~~~~~~~~~~\n");
//		printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
//				matrix.m00, matrix.m01, matrix.m02, matrix.m03,
//				matrix.m10, matrix.m11, matrix.m12, matrix.m13,
//				matrix.m20, matrix.m21, matrix.m22, matrix.m23,
//				matrix.m30, matrix.m31, matrix.m32, matrix.m33);

//		anim_obj->local_transforms[transform0.id] = translate;
//		mat4_translate(&anim_obj->local_transforms[transform0.id], transform.position.x, transform.position.y, transform.position.z);



//		mat4_mul(&anim_obj->local_transforms[transform0.id], &anim_obj->local_transforms[transform0.id], &scale);
//		mat4_mul(&anim_obj->global_transforms[transform0.id], &translate, &rotate);
//		anim_obj->local_transforms[transform0.id] = translate;
//		printf("Boneid: [%d] %d\n", i, transform0.id);
//		mat4_identity(&anim_obj->local_transforms[transform0.id]);
//		anim_obj->local_transforms[transform0.id] = rotate;
//		anim_obj->global_transforms[transform0.id] = translate;
	}
	_cl_calcBoneTransform(anim_obj, &anim_obj->skel.root_bone, &IDENTITY_MATRIX);

}

	cl_r_doRender(dt);
	cl_display_update();

	if(_unloadlevel) {
		_unloadlevel = false;
		_cl_unload_level();
	}
}
