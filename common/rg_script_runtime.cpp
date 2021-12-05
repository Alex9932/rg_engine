/*
 * rg_script_runtime.cpp
 *
 *  Created on: Nov 5, 2021
 *      Author: alex9932
 */

#include "rg_script_runtime.h"
#include "rg_loader.h"

// RUNTIME FUNTIONS
static void rt_include(js_State* state) {
	const char* path = js_tostring(state, 1);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Executing script: %s", path);
	rg_Resource* res = rg_loadResource(path);
	char* source = (char*)rg_malloc(sizeof(char) * (res->length + 1));
	memset(source, '\0', sizeof(char) * (res->length + 1));
	memcpy(source, res->data, res->length);
	js_dostring(state, source);
	rg_free(source);
	rg_freeResource(res);
//	js_dofile(state, local_path);
	js_pushundefined(state);
}

static void rt_print(js_State* state) {
	const char* text = js_tostring(state, 1);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "> %s", text);
	js_pushundefined(state);
}

static void rt_clock(js_State* state) {
	js_pushnumber(state, rg_getRunningTime());
}

void rg_js_initRuntime(rg_jsState* state) {
	rg_js_attachFunction(state, rt_print, "print", 1);
	rg_js_attachFunction(state, rt_include, "include", 1);
	rg_js_attachFunction(state, rt_clock, "clock", 0);
}
