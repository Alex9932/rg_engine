/*
 * cl_guiconsole.cpp
 *
 *  Created on: Oct 28, 2021
 *      Author: alex9932
 */

#include "cl_guiconsole.h"
#include "cl_renderer2d.h"
#include "cl_display.h"
#include "cl_font.h"
#include <rg_engine.h>

static bool renderConsole = false;
static cl_font_t* font;
static const size_t BUFFER_SIZE = 256;
static wchar_t input[BUFFER_SIZE];

bool _callback(rg_Event* event) {
	if(event->type != RG_EVENT_SDL)
		return true;

	if(event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_GRAVE) { // @suppress("Field cannot be resolved")
		renderConsole = !renderConsole;
		return true;
	}

	if(!renderConsole)
		return true;

	if(event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) { // @suppress("Field cannot be resolved")
		input[wcslen(input)-1] = '\0';
		return true;
	}
	if(event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_RETURN) { // @suppress("Field cannot be resolved")
		input[wcslen(input)-1] = '\0';
//		char l_input[BUFFER_SIZE];
//		for (size_t i = 0; i < BUFFER_SIZE; ++i) {
//			l_input[i] = input[i];
//		}

//		wprintf(L">>> %s", input);

//		SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, ">>> %s", l_input);
		memset(input, '\0', sizeof(wchar_t) * BUFFER_SIZE);
		return true;
	}
	if(event->event.type == SDL_TEXTINPUT) {
		if(cl_r2d_strlen(font, input, 1) + 45 > cl_display_getWidth())
			return true;

		wchar_t text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
		for (size_t i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i) {
			if(event->event.text.text[i] != '`')
				text[i] = event->event.text.text[i];

			if(event->event.text.text[i] == '\0')
				break;
		}
		wcscat(input, text);
		return true;
	}

	return true;
}

void cl_gui_console_init() {
	rg_registerEventHandler(_callback);
	font = cl_font_new("platform/f_console.ttf", 15);

	for (size_t i = 0; i < BUFFER_SIZE; ++i) {
		input[i] = '\0';
	}
	input[BUFFER_SIZE-1] = '\0'; // LAST ELEMENT MUST BE '\0'
}

void cl_gui_console_destroy() {
	cl_font_delete(font);
}

void cl_gui_console_update(double dt) {

	if(!renderConsole)
		return;

	float w = cl_display_getWidth();
	float h = 500;

	cl_r2d_rotate({0, 0, 0});
	cl_r2d_translate({0, cl_display_getHeight() - h});

	cl_r2d_bind(0);
	cl_r2d_begin();
	cl_r2d_vertex({0, 0, 0, 0, 0, 0, 0, 0.5});
	cl_r2d_vertex({w, 0, 1, 0, 0, 0, 0, 0.5});
	cl_r2d_vertex({w, h, 1, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({w, h, 1, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({0, h, 0, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({0, 0, 0, 0, 0, 0, 0, 0.5});
	cl_r2d_end();


	cl_r2d_drawString(font, L">>>", 5, 5, 1, 0.85, 0.85, 0.85, 1);
	cl_r2d_drawString(font, input, 33, 5, 1, 0.85, 0.85, 0.85, 1);

	if((int)rg_getRunningTime() % 2 == 1) {
		cl_r2d_drawString(font, L"_", 33 + cl_r2d_strlen(font, input, 1), 5, 1, 0.85, 0.85, 0.85, 1);
	}

}
