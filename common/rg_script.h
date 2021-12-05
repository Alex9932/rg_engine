/*
 * rg_script.h
 *
 *  Created on: Nov 2, 2021
 *      Author: alex9932
 */

#ifndef RG_SCRIPT_H_
#define RG_SCRIPT_H_

#include "rg_engine.h"
#include <mujs.h>

typedef struct rg_jsState {
	js_State* state;
	char exec_path[128];
} rg_jsState;

#define JS_GLOBALSTRING(state, name, str) js_newstring(state, str);js_setglobal(state, name)

rg_jsState rg_js_makeDefaultState();
void rg_js_attachFunction(rg_jsState* state, void(*funcptr)(js_State*), rg_string name, int argc);
void rg_js_callfunc(rg_jsState* state, rg_string name);
void rg_js_execute(rg_jsState* state, rg_string path);

#endif /* RG_SCRIPT_H_ */
