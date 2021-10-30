/*
 * cl_gui.cpp
 *
 *  Created on: Aug 30, 2021
 *      Author: alex9932
 */

#include "cl_gui.h"

#include <locale>
#include <codecvt>
#include <string>

#include "cl_renderer2d.h"
#include "cl_texture.h"
#include "cl_display.h"
#include "cl_renderer.h"
#include "client.h"

static bool is_render = false;


/////////////
static cl_texture_t l_bg;
static cl_texture_t l_prog;
/////////////

void cl_gui_showLoadingScreen() {
	is_render = true;
	l_bg = cl_getTexture("gamedata/textures/gui/load_bg.png");
	l_prog = cl_getTexture("gamedata/textures/gui/loading.png");
}

void cl_gui_hide() {
	is_render = false;
}

static float angle = 0;

void cl_gui_draw(double dt) {
	if(is_render) {
		float w = cl_display_getWidth(), h = cl_display_getHeight();
		cl_r2d_rotate({0, 0, 0});
		cl_r2d_translate({0, 0});

		cl_r2d_bind(l_bg);
		cl_r2d_begin();
		cl_r2d_vertex({0, 0, 0, 0, 1, 1, 1, 1});
		cl_r2d_vertex({w, 0, 1, 0, 1, 1, 1, 1});
		cl_r2d_vertex({w, h, 1, 1, 1, 1, 1, 1});
		cl_r2d_vertex({w, h, 1, 1, 1, 1, 1, 1});
		cl_r2d_vertex({0, h, 0, 1, 1, 1, 1, 1});
		cl_r2d_vertex({0, 0, 0, 0, 1, 1, 1, 1});
		cl_r2d_end();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(LOADING_STATUS);
		cl_r2d_drawString(cl_r_getDefaultFont(), wide.c_str(), 15, 15, 0.5, 1, 1, 1, 1);

		angle += 15.28f * dt;
		cl_r2d_rotate({0, 0, angle});
		cl_r2d_translate({100, 100});

		cl_r2d_bind(l_prog);
		cl_r2d_begin();
		cl_r2d_vertex({-25, -25, 0, 0, 1, 1, 1, 1});
		cl_r2d_vertex({ 25, -25, 1, 0, 1, 1, 1, 1});
		cl_r2d_vertex({ 25,  25, 1, 1, 1, 1, 1, 1});
		cl_r2d_vertex({ 25,  25, 1, 1, 1, 1, 1, 1});
		cl_r2d_vertex({-25,  25, 0, 1, 1, 1, 1, 1});
		cl_r2d_vertex({-25, -25, 0, 0, 1, 1, 1, 1});
		cl_r2d_end();
	}
}
