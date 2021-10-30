/*
 * engine.h
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_ENGINE_H_
#define RG_ENGINE_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
//#include "rg_level.h"

#define RG_VERSION    "0.04.1"
//#define RG_BUILD_DATE "30.05.2021 at 13:02"
#define RG_BUILD_DATE "20.10.2021 at 20:07"

#define RG_MESHES_DIR "meshes"
#define RG_MATERIALS_DIR "materials"

#define RG_CLIENT          0b00000001
#define RG_DEBUG           0b10000000

#define SDL_LOG_CATEGORY_CLIENT  SDL_LOG_CATEGORY_RESERVED1
#define SDL_LOG_CATEGORY_SERVER  SDL_LOG_CATEGORY_RESERVED2
#define SDL_LOG_CATEGORY_NETWORK SDL_LOG_CATEGORY_RESERVED3
#define SDL_LOG_CATEGORY_DEBUG   SDL_LOG_CATEGORY_RESERVED4

#ifdef _WIN32
#define RG_INLINE inline
#else
#define RG_INLINE inline __attribute__((always_inline))
#endif

#define RG_CHECKFLAG(var, flag) ((var & flag) == flag)
#define rg_print  SDL_Log
#define rg_assert SDL_assert_always

enum RG_EventType {
	RG_EVENT_SDL       = 0,
	RG_EVENT_SHUTDOWN  = 1,
	RG_EVENT_UPDATE    = 2,
	RG_EVENT_LOADLEVEL = 3
};

typedef struct rg_Event {
	RG_EventType type;
	SDL_Event event;
} rg_Event;

typedef void (*UpdateFunc)(double);
typedef bool (*EventCallback)(rg_Event*);

typedef const char*    rg_string;
typedef const wchar_t* rg_wstring;


extern bool rg_isDebug;
extern double rg_fps_avg;
extern double rg_fps_max;
extern struct rg_level_t* rg_level;
extern rg_string rg_fsjson;
extern rg_string rg_s_port;
extern rg_string rg_c_addr;
extern rg_string rg__level;

// Mem alloc
void*  rg_malloc(size_t size);
void*  rg_calloc(size_t num, size_t size);
void*  rg_realloc(void* ptr, size_t size);
void   rg_free(void* ptr);
size_t rg_getAllocatedMem();

//
void rg_init(int argc, rg_string* argv);
void rg_start();
void rg_stop();
bool rg_isRunning();

double rg_getRunningTime();

void rg_registerEventHandler(EventCallback callback);
void rg_pushEvent(rg_Event* event);
void rg_buildResourcePath(const char* levelname, const char* name, char* dest, const char* type);

#endif /* RG_ENGINE_H_ */
