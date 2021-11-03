/*
 * rg_script.cpp
 *
 *  Created on: Nov 2, 2021
 *      Author: alex9932
 */

#include "rg_script.h"
#include "rg_loader.h"
#include "rg_string.h"
#include <SDL2/SDL.h>

static void _include (js_State* state) {
	const char* local_path = js_tostring(state, 1);

	js_getglobal(state, "exec_path");
	const char* exec_path = js_tostring(state, 1);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Including from: %s %s", exec_path, local_path);

//	js_dofile(state, local_path);
	js_pushundefined(state);
}

jsState js_makeDefaultState() {
	jsState state;
	state.state = js_newstate(NULL, NULL, JS_STRICT);
	js_attachFunction(&state, _include, "include", 1);

//	JS_GLOBALSTRING(state, "_G_MUJS_WRAPPER_VER", _G_MUJS_WRAPPER_VER);
	return state;
}

void js_attachFunction(jsState* state, void(*funcptr)(js_State*), rg_string name, int argc) {
	js_newcfunction(state->state, funcptr, name, argc);
	js_setglobal(state->state, name);
}

void js_callfunc(jsState* state, rg_string name) {
	js_pushnull(state->state);
	js_getglobal(state->state, name);
	if (js_pcall(state->state, 0)) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "An exception occurred in the javascript function: %s\n", name);
	}
	js_pop(state->state, 1);
}

void js_execute(jsState* state, rg_string path) {
	memset(state->exec_path, '\0', 128);
	size_t pos = rg_strfind_end(path, '/');
	memcpy(state->exec_path, path, pos + 1);
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Path: %s", state->exec_path);
	JS_GLOBALSTRING(state->state, "exec_path", state->exec_path);

	rg_Resource* res = rg_loadResource(path);

	rg_string code = (rg_string)res->data;
	js_dostring(state->state, code);

	rg_freeResource(res);
}
