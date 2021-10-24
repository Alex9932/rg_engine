/*
 * cl_fbo.h
 *
 *  Created on: 20 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_FBO_H_
#define CL_FBO_H_

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define FBO_DEPTH 0x00000001 // Attach depth buffer to FBO
#define FBO_RB_DS 0x00000010 // Use renderbuffer for depth attachment

typedef struct Framebuffer {
	char flags;
	Uint32 width;
	Uint32 height;
	GLuint fbo;
	GLuint depth;
	GLuint color[6];
} Framebuffer;

Framebuffer* cl_fboNew(Uint32 width, Uint32 height, Uint8 attachments, char flags); // Uint8 attachments  ~ ~ ~ (MAX 6) ~ ~ ~
void cl_fboFree(Framebuffer* fbo);
void cl_fboBind(Framebuffer* fbo);

#endif /* CL_FBO_H_ */
