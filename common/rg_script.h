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

typedef struct jsState {
	js_State* state;
	char exec_path[128];
} jsState;

#define JS_GLOBALSTRING(state, name, str) js_newstring(state, str);js_setglobal(state, name)

jsState js_makeDefaultState();
void js_attachFunction(jsState* state, void(*funcptr)(js_State*), rg_string name, int argc);
void js_callfunc(jsState* state, rg_string name);
void js_execute(jsState* state, rg_string path);

#endif /* RG_SCRIPT_H_ */
