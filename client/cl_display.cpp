/*
 * cl_display.cpp
 *
 *  Created on: 3 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_display.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <rg_engine.h>

static SDL_Window* _startup_window = NULL;
static SDL_Window* _w_window_ptr   = NULL;
static SDL_GLContext _w_glcontext  = NULL;

//static uint _w_init_width     = 800;
//static uint _w_init_height    = 600;
static uint _w_init_width     = 1280;
static uint _w_init_height    = 720;
static uint _w_current_width  = 0;
static uint _w_current_height = 0;
static bool _w_fullscreen = false;
static bool _m_grabbed = false;

static float _swp_int = 0;

void cl_display_init() {
	// Use OpenGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Set 0 to disable frame-rate limiter
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	if(!rg_isDebug) {
		_startup_window = SDL_CreateWindow("rgEngine - init", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 300, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
		SDL_Renderer *ren = SDL_CreateRenderer(_startup_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_Surface *bmp = SDL_LoadBMP("platform/logo.bmp");
		SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
		SDL_FreeSurface(bmp);
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex, NULL, NULL);
		SDL_RenderPresent(ren);
	}
}

void cl_display_open() {
	_w_current_width = _w_init_width;
	_w_current_height = _w_init_height;

	_w_window_ptr = SDL_CreateWindow("rgEngine", 5, 5, 650, 500, SDL_WINDOW_OPENGL);
	SDL_assert(_w_window_ptr);
	_w_glcontext = SDL_GL_CreateContext(_w_window_ptr);
	if (!_w_glcontext) {
		SDL_Log("[engine] Error!");
	}
	SDL_GL_SwapWindow(_w_window_ptr);

	SDL_Delay(500);
	if(!rg_isDebug) {
		SDL_DestroyWindow(_startup_window);
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		SDL_Log("[engine] Error initializing GLEW! %s", glewGetErrorString(glewError));
	}

	SDL_SetWindowSize(_w_window_ptr, _w_current_width, _w_current_height);
	SDL_SetWindowPosition(_w_window_ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	glViewport(0, 0, _w_current_width, _w_current_height);
	SDL_GL_SetSwapInterval(_swp_int);
}

void cl_display_close() {
	SDL_GL_DeleteContext(_w_glcontext);
	SDL_DestroyWindow(_w_window_ptr);
}

void cl_display_update() {
	SDL_GL_SetSwapInterval(_swp_int);
	SDL_bool b = _m_grabbed ? SDL_TRUE : SDL_FALSE;
	SDL_SetWindowGrab(_w_window_ptr, b);
	SDL_CaptureMouse(b);
	SDL_SetRelativeMouseMode(b);
	SDL_GL_SwapWindow(_w_window_ptr);
}

void cl_display_toggleFullscreen() {
	_w_fullscreen = !_w_fullscreen;
	if (_w_fullscreen) {
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		_w_current_width = DM.w;
		_w_current_height = DM.h;
	} else {
		_w_current_width = _w_init_width;
		_w_current_height = _w_init_height;
	}

	SDL_SetWindowSize(_w_window_ptr, _w_current_width, _w_current_height);
	SDL_SetWindowFullscreen(_w_window_ptr, _w_fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void cl_display_setSwapInterval(float interval) {
	_swp_int = interval;
}

void cl_display_setMouseGrabbed(bool b) {
	_m_grabbed = b;
}

float cl_display_getAspect() {
	return (float)_w_current_width / (float)_w_current_height;
}

float cl_display_getWidth() {
	return _w_current_width;
}

float cl_display_getHeight() {
	return _w_current_height;
}
