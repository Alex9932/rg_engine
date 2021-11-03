/*
 * rg_string.cpp
 *
 *  Created on: 9 апр. 2021 г.
 *      Author: alex9932
 */

#include "rg_string.h"

static rg_string_t* rg_string_new(Uint16 length) {
	rg_string_t* str = (rg_string_t*)rg_malloc(sizeof(rg_string_t));
//	str->data = (wchar_t*)rg_malloc(sizeof(wchar_t) * (length + 1));
//	str->length = length + 1;
//	SDL_memset(str->data, '\0', str->length);
	str->data = (wchar_t*)rg_malloc(sizeof(wchar_t) * length);
	str->length = length;
	return str;
}

rg_string_t* rg_strdup(rg_string str) {
	rg_string_t* string = rg_string_new(SDL_strlen(str));
//	for (int i = 0; i < string->length - 1; i++) {
	for (int i = 0; i < string->length; i++) {
		string->data[i] = str[i]; // char => wchar_t
	}
	return string;
}

rg_string_t* rg_wstrdup(rg_wstring str) {
	rg_string_t* string = rg_string_new(SDL_wcslen(str));
	SDL_memcpy(string->data, str, sizeof(wchar_t) * string->length);
	return string;
}

rg_string_t* rg_strcat(rg_string_t* left, rg_string_t* right) {
	rg_string_t* string = rg_string_new(left->length + right->length);
	SDL_memcpy(string->data, left->data, sizeof(wchar_t) * left->length);
	SDL_memcpy(string->data + left->length, right->data, sizeof(wchar_t) * right->length);
	return string;
}

bool rg_streql(rg_string left, rg_string right) {
	size_t lenpre = strlen(left);
	size_t lenstr = strlen(right);
	return ( (lenstr == lenpre) && (SDL_memcmp(left, right, lenpre) == 0) ) ? true : false;
}

bool rg_streql(rg_string_t* left, rg_string_t* right) {
	size_t lenpre = left->length;
	size_t lenstr = right->length;
	return ( (lenstr == lenpre) && (SDL_memcmp(left->data, right->data, lenpre) == 0) ) ? true : false;
}

bool rg_streql(rg_string_t* left, rg_wstring right) {
	size_t lenpre = left->length;
	size_t lenstr = SDL_wcslen(right);
	return ( (lenstr == lenpre) && (SDL_memcmp(left->data, right, sizeof(wchar_t) * lenpre) == 0) ) ? true : false;
}

Uint16 rg_strlen(rg_string_t* str) {
	return str->length;
}

void rg_strfree(rg_string_t* str) {
	rg_free(str->data);
	rg_free(str);
}

void rg_strprint(rg_string_t* str, FILE* f) {
	fprintf(f, "%ls\n", str->data);
}

size_t rg_strfind(rg_string str, char c) {
	for (size_t i = 0; i < SDL_strlen(str); ++i) {
		if(str[i] == c)
			return i;
	}

	return 0;
}

size_t rg_strfind_end(rg_string str, char c) {
	for (size_t i = SDL_strlen(str); i >= 0; --i) {
		if(str[i] == c)
			return i;
	}

	return 0;
}

size_t rg_wstrfind(rg_wstring str, wchar c) {
	for (size_t i = 0; i < SDL_wcslen(str); ++i) {
		if(str[i] == c)
			return i;
	}

	return 0;
}

size_t rg_wstrfind_end(rg_wstring str, wchar c) {
	for (size_t i = SDL_wcslen(str); i >= 0; --i) {
		if(str[i] == c)
			return i;
	}

	return 0;
}
