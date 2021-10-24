/*
 * rg_string.h
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_STRING_H_
#define RG_STRING_H_

#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "rg_engine.h"

typedef struct rg_string_t {
	wchar_t* data;
	Uint16 length;
} rg_string_t;

rg_string_t* rg_strdup(rg_string str);
rg_string_t* rg_wstrdup(rg_wstring str);
rg_string_t* rg_strcat(rg_string_t* left, rg_string_t* right);
bool rg_streql(rg_string left, rg_string right);
bool rg_streql(rg_string_t* left, rg_string_t* right);
bool rg_streql(rg_string_t* left, rg_wstring right);
Uint16 rg_strlen(rg_string_t* str);
void rg_strfree(rg_string_t* str);
void rg_strprint(rg_string_t* str, FILE* f);

#endif /* RG_STRING_H_ */
