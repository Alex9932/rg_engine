/*
 * cl_texture.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#define STB_IMAGE_IMPLEMENTATION
#define LOAD_FROM_RG_RESOURCE

#include "cl_texture.h"
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <map>

//static std::map<rg_string, cl_texture_t> textures;

cl_texture_t cl_getTexture(rg_string path) {
//	if (textures[path]) {
//		return textures[path];
//	}

	cl_texture_t texture;

	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Load texture: %s", path);

	int channels = 4;
	int width = 0;
	int height = 0;
	unsigned char* data = NULL;

#ifdef LOAD_FROM_RG_RESOURCE
	rg_Resource* res = rg_loadResource(path);
	data = stbi_load_from_memory((stbi_uc const*)res->data, res->length, &width, &height, &channels, 0);
#else
	data = stbi_load(path, &width, &height, &channels, 0);
#endif

	if (data == NULL) {
		SDL_Log("Unable to load texutre: %s", path);
	}
	assert(data);

	GLenum format = GL_RGBA;
	GLenum type = GL_RGBA;
	if (channels == 3) {
		format = GL_RGB;
		type = GL_RGB;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, type, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);

	if (GLEW_EXT_texture_filter_anisotropic) {
		GLfloat max = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, SDL_max(4.0, max));
	} else {
		SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Not supported!");
	}

	stbi_image_free(data);

#ifdef LOAD_FROM_RG_RESOURCE
	rg_freeResource(res);
#endif

//	textures[path] = texture;
	return texture;
}

void cl_deleteTexture(cl_texture_t texture) {
	glDeleteTextures(1, &texture);

	// Remove texture from map
//	for (auto it = textures.begin(); it != textures.end();) {
//		if (it->second == texture) {
//			it = textures.erase(it);
//		} else {
//			++it;
//		}
//	}
}
