/*
 * cl_font.h
 *
 *  Created on: 27 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_FONT_H_
#define CL_FONT_H_

#include <rg_engine.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#define F_ATLAS_WIDTH  32
#define F_ATLAS_HEIGHT 64

typedef struct cl_char_t {
	int size_x;
	int size_y;
	int bearing_x;
	int bearing_y;
	Uint32 advance_x;
	Uint32 advance_y;
} cl_char_t;

typedef struct cl_font_t {
	GLuint atlas;
	Uint32 f_size;
	cl_char_t chars[F_ATLAS_WIDTH * F_ATLAS_HEIGHT];
} cl_font_t;

void cl_font_init();
void cl_font_destroy();

cl_font_t* cl_font_new(rg_string file, Uint32 size);
void cl_font_delete(cl_font_t* font);

#endif /* CL_FONT_H_ */
