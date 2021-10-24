/*
 * cl_fbo.cpp
 *
 *  Created on: 20 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_fbo.h"
#include <rg_engine.h>

GLuint _fbo_makeTexAttachment(GLuint attachment, int width, int height, GLenum internalType, GLenum texType, GLenum type) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalType, width, height, 0, texType, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
	return texture;
}

Framebuffer* cl_fboNew(Uint32 width, Uint32 height, Uint8 attachments, char flags) {
	Framebuffer* fbo = (Framebuffer*)rg_malloc(sizeof(Framebuffer));
	SDL_memset(fbo, 0, sizeof(Framebuffer));

	fbo->flags = flags;
	fbo->width = width;
	fbo->height = height;

	glGenFramebuffers(1, &fbo->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);

	GLenum* color_buffers = (GLenum*)rg_malloc(sizeof(GLenum) * attachments);
	for(int i = 0; i < attachments; i++) {
		color_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		fbo->color[i] = _fbo_makeTexAttachment(color_buffers[i], fbo->width, fbo->height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	}

	if(RG_CHECKFLAG(flags, FBO_DEPTH)) {
		if(RG_CHECKFLAG(flags, FBO_RB_DS)) {
			glGenRenderbuffers(1, &fbo->depth);
			glBindRenderbuffer(GL_RENDERBUFFER, fbo->depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo->width, fbo->height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depth);
		} else {
			fbo->depth = _fbo_makeTexAttachment(GL_DEPTH_ATTACHMENT, fbo->width, fbo->height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
		}
	}

	glDrawBuffers(attachments, color_buffers);
	rg_free(color_buffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Framebuffer error! Status: 0x%x\n", status);
		cl_fboFree(fbo);
		fbo = NULL;
	}

	return fbo;
}

void cl_fboFree(Framebuffer* fbo) {
	rg_free(fbo);
}

void cl_fboBind(Framebuffer* fbo) {
	if(fbo == 0) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	else { glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo); }
}
