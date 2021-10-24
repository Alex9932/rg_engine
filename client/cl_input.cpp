/*
 * cl_input.cpp
 *
 *  Created on: 14 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_input.h"
#include <rg_engine.h>

static double _m_x  = 0.0;
static double _m_y  = 0.0;
static double _m_dx = 0.0;
static double _m_dy = 0.0;
static double _m_tdx = 0.0;
static double _m_tdy = 0.0;
static double _m_sens = 0.07;

static bool _k_pressed[1024];
static bool _m_pressed[256];

static bool _cl_input(rg_Event* event) {
	if(event->type == RG_EVENT_SDL) {
		if(event->event.type == SDL_MOUSEMOTION) {
			_m_tdx += ((double)event->event.motion.x - _m_x) * _m_sens;
			_m_tdy += ((double)event->event.motion.y - _m_y) * _m_sens;
			_m_x = (double)event->event.motion.x;
			_m_y = (double)event->event.motion.y;
		}

		if(event->event.type == SDL_KEYDOWN || event->event.type == SDL_KEYUP) {
			_k_pressed[event->event.key.keysym.scancode] = (event->event.type == SDL_KEYUP ? false : true);
		}

		if(event->event.type == SDL_MOUSEBUTTONDOWN || event->event.type == SDL_MOUSEBUTTONUP) {
			_m_pressed[event->event.button.button] = (event->event.type == SDL_MOUSEBUTTONUP ? false : true);
		}
	}
	return true;
}

void cl_inputInit() {
	rg_registerEventHandler(_cl_input);
	SDL_memset(_k_pressed, 0, sizeof(_k_pressed));
	SDL_memset(_m_pressed, 0, sizeof(_m_pressed));
}

void cl_inputUpdate() {
	_m_dx = _m_tdx;
	_m_dy = _m_tdy;
	_m_tdx = 0;
	_m_tdy = 0;
}

bool cl_isKeyPressed(Uint16 key) {
	return _k_pressed[key];
}

bool cl_isButtonPressed(Uint8 button) {
	return _m_pressed[button];
}

double cl_getMouseDX() {
	return _m_dx * _m_sens;
}

double cl_getMouseDY() {
	return _m_dy * _m_sens;
}

double cl_getMouseSensitivity() {
	return _m_sens;
}

void cl_setMouseSensitivity(double sens) {
	_m_sens = sens;
}
