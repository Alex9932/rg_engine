/*
 * cl_texture.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#define STBI_MALLOC(sz)           rg_malloc(sz)
#define STBI_REALLOC(p,newsz)     rg_realloc(p,newsz)
#define STBI_FREE(p)              rg_free(p)

#define STB_IMAGE_IMPLEMENTATION

#include "cl_texture.h"
#include <stb/stb_image.h>
#include <GL/glew.h>
#include <rg_loader.h>
//#include <map>

//static std::map<rg_string, cl_texture_t> textures;

cl_texture_t cl_getTexture(rg_string path) {
	return cl_getTexture(path, CL_TEXTURE_DIFFUSE);
}

cl_texture_t cl_getTexture(rg_string path, cl_texture_type type) {
//	if (textures[path]) {
//		return textures[path];
//	}

	cl_texture_t texture;

	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Load texture: %s", path);

	int channels = 4;
	int width = 0;
	int height = 0;
	unsigned char* data = NULL;

	rg_Resource* res = rg_loadResource(path);
	if (res == NULL) {
		SDL_Log("Unable to load texutre: %s, USING DEFAULT!", path);

		switch (type) {
			case CL_TEXTURE_DIFFUSE:
				res = rg_loadResource("gamedata/textures/default/diff.png");
				break;
			case CL_TEXTURE_NORMAL:
				res = rg_loadResource("gamedata/textures/default/norm.png");
				break;
			case CL_TEXTURE_METALLIC:
				res = rg_loadResource("gamedata/textures/default/metl.png");
				break;
			case CL_TEXTURE_ROUGHNESS:
				res = rg_loadResource("gamedata/textures/default/roug.png");
				break;
			default:
				break;
		}
	}

	if (res->data == NULL) {
		SDL_Log("Unable to load texutre: %s! Loading cannot be continue!", path);
	}

	data = stbi_load_from_memory((stbi_uc const*)res->data, res->length, &width, &height, &channels, 0);
	rg_assert(data);

	GLenum format = GL_RGBA;
	GLenum ftype = GL_RGBA;
	if (channels == 3) {
		format = GL_RGB;
		ftype = GL_RGB;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, ftype, GL_UNSIGNED_BYTE, data);

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

	rg_freeResource(res);

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
