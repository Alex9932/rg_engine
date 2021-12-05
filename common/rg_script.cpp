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

#include "rg_script_runtime.h"

rg_jsState rg_js_makeDefaultState() {
	rg_jsState state;
	state.state = js_newstate(NULL, NULL, JS_STRICT);

	rg_js_initRuntime(&state);

//	JS_GLOBALSTRING(state, "_G_MUJS_WRAPPER_VER", _G_MUJS_WRAPPER_VER);
	return state;
}

void rg_js_attachFunction(rg_jsState* state, void(*funcptr)(js_State*), rg_string name, int argc) {
	js_newcfunction(state->state, funcptr, name, argc);
	js_setglobal(state->state, name);
}

void rg_js_callfunc(rg_jsState* state, rg_string name) {
	js_pushnull(state->state);
	js_getglobal(state->state, name);
	if (js_pcall(state->state, 0)) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "An exception occurred in the javascript function: %s\n", name);
	}
	js_pop(state->state, 1);
}

void rg_js_execute(rg_jsState* state, rg_string path) {
	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Executing script: %s", path);

	memset(state->exec_path, '\0', 128);
	size_t pos = rg_strfind_end(path, '/');
	memcpy(state->exec_path, path, pos + 1);
	JS_GLOBALSTRING(state->state, "exec_path", state->exec_path);

	rg_Resource* res = rg_loadResource(path);

	rg_string code = (rg_string)res->data;
	js_dostring(state->state, code);

	rg_freeResource(res);
}
