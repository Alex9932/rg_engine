/*
 * shader.cpp
 *
 *  Created on: Jul 28, 2020
 *      Author: alex9932
 */


#include "cl_shader.h"

#include <rg_engine.h>
#include <rg_loader.h>
#include <SDL2/SDL.h>
#include <assert.h>

static void __shader_make(rg_string shader_file, rg_string name, GLuint shader, GLuint program) {
	rg_Resource* data = rg_loadResource(shader_file);
	char* str = (char*)data->data;
	GLint len = data->length;

	glShaderSource(program, 1, &str, &len);
	glCompileShader(program);

	int  success;
	char error[512];
	glGetShaderiv(program, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(program, 512, NULL, error);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Shader error in %s\n%s\n", name, error);
	}

	glAttachShader(shader, program);
	rg_freeResource(data);
}

/** PUBLIC **/

rg_Shader shader_create(rg_string vs, rg_string fs, rg_string gs) {
	assert(vs);
	assert(fs);

	rg_Shader shader = glCreateProgram();
	uint g_vs, g_fs, g_gs;

	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Loading shaders: %s %s", vs, fs);
	g_vs = glCreateShader(GL_VERTEX_SHADER);
	g_fs = glCreateShader(GL_FRAGMENT_SHADER);

	if (gs != nullptr) {
		g_gs = glCreateShader(GL_GEOMETRY_SHADER);
		SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "|___GEOMETRY: %s", gs);
	} else {
		g_gs = 0xFFFFFFFF;
	}

	__shader_make(vs, "vertex", shader, g_vs);
	__shader_make(fs, "fragment", shader, g_fs);
	if (g_gs != 0xFFFFFFFF) {
		SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "SHADER USES GEOMETRY PROGRAM!");
		__shader_make(gs, "geometry", shader, g_gs);
	}

	glLinkProgram(shader);
	int  success;
	char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Shader linking failed!\n%s\n", infoLog);
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Shader validation failed!\n%s\n", infoLog);
	}

	glDeleteShader(g_vs);
	glDeleteShader(g_fs);
	if (g_gs != 0xFFFFFFFF) {
		glDeleteShader(g_gs);
	}

	return shader;
}
