/*
 * engine.cpp
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: alex9932
 */

#include "rg_engine.h"
#include <map>
#include <vector>
#include <stdio.h>
#include <SDL2/SDL_net.h>
#include <cJSON.h>
#include "rg_net.h"
#include "rg_string.h"

#include "rg_filesystem.h"

#include <server.h>
#include <client.h>

static FILE* _e_log_file;
static bool running = false;
static bool event_running = false;
static double _wd_event_timer = 0;

static double _fps[10];
double rg_fps_avg = 0;
double rg_fps_max = -1.0;
bool rg_isDebug;
rg_string rg_fsjson;
rg_string rg_s_port;
rg_string rg_c_addr;
rg_string rg__level;

struct rg_level_t* rg_level = NULL;

// RG_CLIENT | RG_DEBUG
static Uint32 _flags = 0;

static SDL_Thread* _e_thr_event;
static SDL_Thread* _e_thr_wd;
static std::vector<EventCallback> _callbacks;

static UpdateFunc g_update_func;
static rg_Event sd_event; // Reserved event

//  ~ ~ ~ Memory allocation ~ ~ ~  //
static std::map<void*, size_t> alloc_table;

static void _rg_alloc_fail() {
	SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Out of memory!");
}

RG_INLINE static void _rg_checkmem() {
//	int num = SDL_GetNumAllocations();
//	SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "Allocations: %d", num);
}

void* rg_malloc(size_t size) {
	void* ptr = SDL_malloc(size);
	alloc_table[ptr] = size;
	if(ptr == NULL) {
		_rg_alloc_fail();
	}
	_rg_checkmem();
	return ptr;
}

void* rg_calloc(size_t num, size_t size) {
	void* ptr = rg_malloc(num * size);
	memset(ptr, 0, num * size);
	return ptr;
}

void* rg_realloc(void* ptr, size_t size) {
	void* _ptr = SDL_realloc(ptr, size);
	alloc_table[ptr] = size;
	if(_ptr == NULL) {
		_rg_alloc_fail();
	}
	_rg_checkmem();
	return _ptr;
}

void rg_free(void* ptr) {
	alloc_table.erase(ptr);
	SDL_free(ptr);
	_rg_checkmem();
}

size_t rg_getAllocatedMem() {
	size_t mem = 0;
	for (auto it = alloc_table.begin(); it != alloc_table.end(); ++it) {
		mem += it->second;
	}
	return mem;
}

//  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  //

static const char* _e_log_getCat(int category) {
	switch(category) {
		case SDL_LOG_CATEGORY_APPLICATION: return " game ";
		case SDL_LOG_CATEGORY_TEST:        return " test ";
		case SDL_LOG_CATEGORY_CUSTOM:      return " game ";
		case SDL_LOG_CATEGORY_ASSERT:      return "assert";
		case SDL_LOG_CATEGORY_SYSTEM:      return "system";
		case SDL_LOG_CATEGORY_RENDER:      return "render";
		case SDL_LOG_CATEGORY_CLIENT:      return "client";
		case SDL_LOG_CATEGORY_SERVER:      return "server";
		case SDL_LOG_CATEGORY_NETWORK:     return "network";
		case SDL_LOG_CATEGORY_ERROR:       return "error";
		case SDL_LOG_CATEGORY_AUDIO:       return "audio";
		case SDL_LOG_CATEGORY_VIDEO:       return "video";
		case SDL_LOG_CATEGORY_INPUT:       return "input";
		case SDL_LOG_CATEGORY_DEBUG:       return "debug";
		default: return "app";
	}
}

static const char* _e_log_getPriority(SDL_LogPriority priority) {
	switch(priority) {
		case SDL_LOG_PRIORITY_VERBOSE:  return "Verb";
		case SDL_LOG_PRIORITY_DEBUG:    return " Dbg";
		case SDL_LOG_PRIORITY_INFO:     return "Info";
		case SDL_LOG_PRIORITY_WARN:     return "Warn";
		case SDL_LOG_PRIORITY_ERROR:    return " Err";
		case SDL_LOG_PRIORITY_CRITICAL: return "Crit";
		default: return " Msg";
	}
}

static const char* _e_getCpuName(int cores) {
	switch(cores) {
		case 0: return "WTF?";
		case 1: return "Single-core";
		case 2: return "Dual-core";
		case 3: return "Triple-core";
		case 4: return "Quad-core";
		case 5: return "5 cores";
		case 6: return "6 cores";
		case 7: return "7 cores";
		case 8: return "Octal-core";
		case 9: return "9 cores";
		case 10: return "10 cores";
		case 11: return "11 cores";
		case 12: return "12 cores";
		case 13: return "13 cores";
		case 14: return "14 cores";
		case 15: return "15 cores";
		case 16: return "16 cores";
		case 17: return "17 cores";
		case 18: return "18 cores";
		case 19: return "19 cores";
		case 20: return "20 cores";
		default: return "Unknown";
	}
}

static void _e_log_func(void* userdata, int category, SDL_LogPriority priority, const char* message) {
	if ((category == SDL_LOG_CATEGORY_DEBUG || priority == SDL_LOG_PRIORITY_DEBUG) && !rg_isDebug) { return; }

	const char* cat = _e_log_getCat(category);
	const char* p = _e_log_getPriority(priority);
//	printf("%s [engine] [%s] %s\n", p, cat, message);
	printf("%s [%s] %s\n", p, cat, message);
//	printf("[engine] [%s] %s %s\n", cat, p, message);
	if(_e_log_file) {
//		fprintf(_e_log_file, "%s [engine] [%s] %s\n", p, cat, message);
		fprintf(_e_log_file, "%s [%s] %s\n", p, cat, message);
//		fprintf(_e_log_file, "[engine] [%s] %s %s\n", cat, p, message);
	}
}

static SDL_AssertState _rg_assertion_handler(const SDL_AssertData* data, void* userdata) {
	SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Assertion detected!");
	SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "Info: %s at %s -> %s(%d)", data->condition, data->filename, data->function, data->linenum);
	//printf("Assertion info: %s at %s -> %s(%d)\n", data->condition, data->filename, data->function, data->linenum);
	return SDL_ASSERTION_BREAK;
}

static int e_thr_event_func(void* data) {
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Event handler thread started!");
	rg_Event eng_event;
//	eng_event.type = RG_EVENT_SDL;

	while(event_running) {
		eng_event.type = RG_EVENT_UPDATE;
		rg_pushEvent(&eng_event);
		eng_event.type = RG_EVENT_SDL;

		while (SDL_PollEvent(&eng_event.event)) {
//			SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Event handled: %x", eng_event.event.type);
			switch (eng_event.event.type) {
				case SDL_QUIT:
					rg_stop();
					break;
				default:
					break;
			}

			rg_pushEvent(&eng_event);
//			for (Uint32 i = 0; i < _callbacks.size(); ++i) {
//				_callbacks[i](&eng_event);
//			}
		}

		_wd_event_timer = 0;
	}
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Event handler stopped!");
	return 0;
}

static int e_thr_wd(void* data) {
	while(running) {
		SDL_Delay(100);
		_wd_event_timer += 1;
		if(_wd_event_timer > 5) {
			SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, "EVENT WD_TIMEOUT!");
		}

//		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Fps: %lf", avg_fps);
	}
	return 0;
}

void _rg_processArgs(int argc, rg_string* argv) {
	printf("Launched as:");

	for (int i = 0; i < argc; ++i) {
		printf(" %s", argv[i]);
		rg_string_t* arg = rg_strdup(argv[i]);

		if(rg_streql(arg, L"-client") && argc >= i+1) { // client arg
			_flags |= RG_CLIENT;
			rg_c_addr = argv[i + 1];
		} else if(rg_streql(arg, L"-server")) { // server arg

		} else if(rg_streql(arg, L"-debug")) {
			rg_isDebug = true;
		} else if(rg_streql(arg, L"-port") && argc >= i+1) { // port arg
			rg_s_port = argv[i + 1];
		} else if(rg_streql(arg, L"-fs") && argc >= i+1) { // fs config
			rg_fsjson = argv[i + 1];
		} else if(rg_streql(arg, L"-level") && argc >= i+1) { // fs config
			rg__level = argv[i + 1];
		}

		rg_strfree(arg);
	}
	printf("\n");
}

void rg_init(int argc, rg_string* argv) {
	_rg_processArgs(argc, argv);

	Uint32 sdl_flags = 0;
	sdl_flags |= SDL_INIT_EVENTS;
	sdl_flags |= SDL_INIT_TIMER;
	if (RG_CHECKFLAG(_flags, RG_CLIENT)) {
		sdl_flags |= SDL_INIT_AUDIO;
		sdl_flags |= SDL_INIT_VIDEO;
		sdl_flags |= SDL_INIT_JOYSTICK;
		sdl_flags |= SDL_INIT_GAMECONTROLLER;
	}

	SDL_Init(sdl_flags);
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
	_e_log_file = fopen("platform/last.log", "w");
	SDL_LogSetOutputFunction(_e_log_func, NULL);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "rgEngine v%s (%s), %s, cJSON v%s", RG_VERSION, RG_BUILD_DATE, RG_CHECKFLAG(_flags, RG_CLIENT) ? "client" : "server", cJSON_Version());
	if(RG_CHECKFLAG(_flags, RG_DEBUG)) {
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "~ ~ ~ ! ENGINE RUNNED IN DEBUG PROFILE ! ~ ~ ~");
	}
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "SDL initialized with flags: %x", sdl_flags);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Platform: %s, %s CPU, line %d, %d Mb ram.", SDL_GetPlatform(), _e_getCpuName(SDL_GetCPUCount()), SDL_GetCPUCacheLineSize(), SDL_GetSystemRAM());

	running = true;
	event_running = true;
	SDL_memset(&_fps, 0, sizeof(_fps));
	SDL_SetAssertionHandler(_rg_assertion_handler, NULL);

	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Mounting file systems...");
	if(!rg_fsjson) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "No fstab!");
		rg_assert(rg_fsjson);
	}
	rg_Resource* fsgame = rg_loadResource(rg_fsjson);
	rg_assert(fsgame);
	cJSON* root = cJSON_Parse((rg_string)fsgame->data);
	cJSON* fs_array = cJSON_GetObjectItem(root, "fs");
	for (int var = 0; var < cJSON_GetArraySize(fs_array); ++var) {
		rg_string name = cJSON_GetStringValue(cJSON_GetArrayItem(fs_array, var));
		rg_fs_mount(name);
	}
	cJSON_Delete(root);
	rg_freeResource(fsgame);

	rg_netInit();

	if(RG_CHECKFLAG(_flags, RG_CLIENT)) {
		g_update_func = cl_update;
		cl_main();
	} else {
		g_update_func = sv_update;
		sv_main();
	}
	_e_thr_event = SDL_CreateThread(e_thr_event_func, "Event handler", NULL);
	_e_thr_wd = SDL_CreateThread(e_thr_wd, "WD", NULL);
}

static void _rg_shutdown() {
	if(RG_CHECKFLAG(_flags, RG_CLIENT)) {
		cl_quit();
	} else {
		sv_quit();
	}

	event_running = false;
	rg_netQuit();
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Engine: Shutting down...");
	SDL_Quit();
	fclose(_e_log_file);
	exit(0);
}

static Uint64 st_limiter = SDL_GetPerformanceCounter();
static double _updatetime = 0;
static Uint64 st = SDL_GetPerformanceCounter();
static double _frametime = 0;

static void _rg_update() {
	g_update_func(_frametime);


	// !!! rewrite this !!! (use array pointer)
	for(Uint32 i = 9; i > 0; i--) {
		_fps[i] = _fps[i - 1];
	}
	// !!! rewrite this !!!


	_fps[0] = 1.0 / _frametime;
	rg_fps_avg = (_fps[0] + _fps[1] + _fps[2] + _fps[3] + _fps[4] + _fps[5] + _fps[6] + _fps[7] + _fps[8] + _fps[9]) / 10.0;

	_updatetime = (double)((SDL_GetPerformanceCounter() - st_limiter) / (double)SDL_GetPerformanceFrequency()) * 1000;
	double del = ((1000.0/(double)rg_fps_max) - _updatetime);
	if(del >= 0) {
		SDL_Delay(round(del));
	}

	st_limiter = SDL_GetPerformanceCounter();
	_frametime = (double)((SDL_GetPerformanceCounter() - st) / (double)SDL_GetPerformanceFrequency());
	st = SDL_GetPerformanceCounter();
}

void rg_start() {
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Starting engine...");

	rg_level = rg_newLevel();
	if(rg_loadLevel(rg_level, rg__level) == -1) {
		rg_freeLevel(rg_level);
		rg_level = NULL;
	}

	while(running) {
		_rg_update();
	}

//	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Shutdown started!");
	sd_event.type = RG_EVENT_SHUTDOWN;
	rg_pushEvent(&sd_event);
	SDL_Delay(400);
//	_rg_update();

	_rg_shutdown();
}

void rg_stop() {
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Shutdown request detected!");
	running = false;
}

bool rg_isRunning() {
	return running;
}

void rg_registerEventHandler(EventCallback callback) {
	SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "REG: New handler %x", callback);
	_callbacks.push_back(callback);
}

void rg_pushEvent(rg_Event* event) {
	for (Uint32 i = 0; i < _callbacks.size(); ++i) {
		if(event->type == RG_EVENT_SHUTDOWN) {
			SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "Event: %x", _callbacks[i]);
		}
		_callbacks[i](event);
	}
}

rg_Resource* rg_loadResource(rg_string name) {
	rg_Resource* res = rg_fs_getResource(name);

	if(res != NULL) {
		return res;
	}

	FILE* file = fopen(name, "rb");
	if(!file) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Resource %s not found!", name);
		return NULL;
	}

	res = (rg_Resource*)rg_malloc(sizeof(rg_Resource));

	fseek(file, 0, SEEK_END);
	res->length = ftell(file);
	rewind(file);
	res->data = rg_malloc(res->length);
	memset(res->data, ' ', res->length);
	uint readed = fread(res->data, 1, res->length, file);
	if(readed != res->length && !feof(file)) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Error while reading file! Length/Readed: %d, %d", res->length, readed);
	}
	fclose(file);
	return res;
}

void rg_freeResource(rg_Resource* res) {
	rg_free(res->data);
	rg_free(res);
}

void rg_buildResourcePath(const char* levelname, const char* name, char* dest, const char* type) {
	bool isLevelDir = name[0] == '&';
	SDL_memset(dest, '\0', 128);

	if(isLevelDir) {
		strcpy(dest, "gamedata/levels/");
		strcat(dest, levelname);
		strcat(dest, "/");
		strcat(dest, name+1);
	} else {
		strcpy(dest, "gamedata/");
		strcat(dest, type);
		strcat(dest, "/");
		strcat(dest, name);
	}
}
