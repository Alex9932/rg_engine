/*
 * cl_guiwindow.cpp
 *
 *  Created on: Oct 1, 2021
 *      Author: alex9932
 */

#include <cl_guiwindow.h>
#include "cl_renderer2d.h"

Window::Window() {
	this->x = 0;   this->y = 0;
	this->w = 350; this->h = 160;
}

Window::~Window() {
}

void Window::draw(double dt) {
	cl_r2d_begin();
	cl_r2d_vertex( {0, 0, 0, 0, 0.4, 0.4, 0.4, 1} );
	cl_r2d_vertex( {w, 0, 1, 0, 0.4, 0.4, 0.4, 1} );
	cl_r2d_vertex( {w, h, 1, 1, 0.4, 0.4, 0.4, 1} );
	cl_r2d_vertex( {w, h, 1, 1, 0.4, 0.4, 0.4, 1} );
	cl_r2d_vertex( {0, h, 0, 1, 0.4, 0.4, 0.4, 1} );
	cl_r2d_vertex( {0, 0, 0, 0, 0.4, 0.4, 0.4, 1} );

	cl_r2d_vertex( {0, h - 20, 0, 0, 0.1, 0.1, 0.1, 1} );
	cl_r2d_vertex( {w, h - 20, 1, 0, 0.1, 0.1, 0.1, 1} );
	cl_r2d_vertex( {w, h     , 1, 1, 0.1, 0.1, 0.1, 1} );
	cl_r2d_vertex( {w, h     , 1, 1, 0.1, 0.1, 0.1, 1} );
	cl_r2d_vertex( {0, h     , 0, 1, 0.1, 0.1, 0.1, 1} );
	cl_r2d_vertex( {0, h - 20, 0, 0, 0.1, 0.1, 0.1, 1} );
	cl_r2d_end();
}

void Window::eventHandler(rg_Event* event) {
	if(event->type == RG_EVENT_SDL) {
//		SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Event: %d %d", event->event.motion.x, event->event.motion.y);

	}
}
