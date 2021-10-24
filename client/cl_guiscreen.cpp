/*
 * clguiscreen.cpp
 *
 *  Created on: Oct 1, 2021
 *      Author: alex9932
 */

#include <cl_guiscreen.h>
#include "cl_renderer2d.h"
#include "cl_display.h"

GuiScreen::GuiScreen() {
	Window* wnd = new Window();
	wnd->x = 400;
	wnd->y = 20;
	addWindow(new Window());
	addWindow(wnd);
}

GuiScreen::~GuiScreen() {

}

bool GuiScreen::eventHandler(rg_Event* event) {
	rg_Event e;
	SDL_memcpy(&e, event, sizeof(rg_Event));

	for (size_t i = 0; i < windows.size(); ++i) {
		if(e.type == RG_EVENT_SDL) {
			switch (e.event.type) {
				case SDL_MOUSEMOTION:
					e.event.motion.x = event->event.motion.x - windows[i]->x;
					e.event.motion.y = event->event.motion.y - windows[i]->y;
					e.event.motion.y = cl_display_getHeight() - e.event.motion.y;

					if(windows[i]->isGrabbed) {
						windows[i]->x += event->event.motion.xrel;
						windows[i]->y -= event->event.motion.yrel;
					}

					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					e.event.button.x = event->event.button.x - windows[i]->x;
					e.event.button.y = event->event.button.y - windows[i]->y;
					e.event.button.y = cl_display_getHeight() - e.event.motion.y;

					if(event->event.button.x >= windows[i]->x && event->event.button.x < windows[i]->x + windows[i]->w &&
						cl_display_getHeight() - event->event.button.y >= windows[i]->y + windows[i]->h - 20 && cl_display_getHeight() - event->event.button.y < windows[i]->y + windows[i]->h) {
						windows[i]->isGrabbed = event->event.button.state;
					}

					break;
				case SDL_MOUSEWHEEL:
					e.event.wheel.x = event->event.wheel.x - windows[i]->x;
					e.event.wheel.y = event->event.wheel.y - windows[i]->y;
					e.event.wheel.y = cl_display_getHeight() - e.event.motion.y;
					break;
				default:
					break;
			}
		}

		if(e.event.motion.x > 0 && e.event.motion.x < windows[i]->w
				&& e.event.motion.y > 0 && e.event.motion.y < windows[i]->h) {
			windows[i]->eventHandler(&e);
		}
	}

	return true;
}

void GuiScreen::drawScreen(double dt) {
	cl_r2d_bind(0);
	cl_r2d_begin();
	cl_r2d_vertex( {0   , 0  , 0, 0, 1, 1, 1, 0} );
	cl_r2d_vertex( {1280, 0  , 1, 0, 1, 1, 1, 0} );
	cl_r2d_vertex( {1280, 720, 1, 1, 1, 1, 1, 0} );
	cl_r2d_vertex( {1280, 720, 1, 1, 1, 1, 1, 0} );
	cl_r2d_vertex( {0   , 720, 0, 1, 1, 1, 1, 0} );
	cl_r2d_vertex( {0   , 0  , 0, 0, 1, 1, 1, 0} );
	cl_r2d_end();

	for (size_t i = 0; i < windows.size(); ++i) {
		cl_r2d_rotate( {0, 0, 0} );
		cl_r2d_translate( {windows[i]->x, windows[i]->y} );
		windows[i]->draw(dt);
	}
}
