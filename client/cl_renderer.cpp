/*
 * cl_renderer.cpp
 *
 *  Created on: 26 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_renderer.h"

#include <rg_loader.h>
#include <rg_level.h>
#include <GL/glew.h>
#include <string>
#include <cstring>
#include <sstream>

#include "cl_fbo.h"
#include "cl_shader.h"
#include "cl_display.h"
#include "cl_vao.h"

#include "cl_gui.h"

#include "cl_renderer2d.h"
#include "cl_renderer_line3d.h"

#include "cl_material.h"
#include "cl_particle.h"

#include "cl_guiscreen.h"
#include "cl_guiconsole.h"

#include "cl_surfaces.h"
#include "cl_skybox.h"
#include "cl_grass.h"
#include "cl_water.h"


#define SHADOW_SIZE   512
#define MAX_PARTICLES 1024

static const Uint32 inds[] = { 0, 1, 2, 2, 3, 0 };
static const rg_vertex_t vtx[] = {
	{ -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
	{  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
};

static Camera camera;

static Framebuffer* gbuffer;
static rg_Shader s_gbuffer;

static rg_Shader s_shadowqmap;

// Postprocess
static rg_Shader s_lpbuffer;
static Framebuffer* lp_buffer;
static rg_Shader s_combine;
static Framebuffer* cb_buffer;
static rg_Shader s_sslr;
static Framebuffer* sslr_buffer;
static cl_VAO* quad;

static rg_Shader s_dbg_light;
static Framebuffer* dbg_light_buffer;
static cl_VAO* dbg_light_box;

// Point light shadow maps
static GLuint shadow_qubemaps[4];
static GLuint shadow_mapFBO[4];

// Particles
static rg_Shader s_particle;
static GLuint p_vao;
static GLuint p_vbo;

static std::vector<cl_particle> particles;
//static cl_particle _particles[4];


//static GuiScreen* screen;

static bool _changeViewport = false;
static bool _wireframe = false;
static bool _doAnimation = true;
static bool _allowShadows = true;
static bool _drawAxis = true;

static bool allbright = false;
static bool g_cursor = true;
static bool r_canRender = false;

static bool _r_handler(rg_Event* event) {
	if(event->type == RG_EVENT_SDL && event->event.type == SDL_WINDOWEVENT) {
		if(event->event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			//SDL_Log("Resized: %d %d", (int)cl_display_getWidth(), (int)cl_display_getHeight());
			_changeViewport = true;
		}
	}

	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_F5) { // @suppress("Field cannot be resolved")
		allbright = !allbright;
	}
	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_F6) { // @suppress("Field cannot be resolved")
		g_cursor = !g_cursor;
	}
	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_F7) { // @suppress("Field cannot be resolved")
		_allowShadows = !_allowShadows;
	}

	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_F) { // @suppress("Field cannot be resolved")
		_doAnimation = !_doAnimation;
	}
	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_G) { // @suppress("Field cannot be resolved")
		_wireframe = !_wireframe;
	}

	if(event->type == RG_EVENT_SDL && event->event.type == SDL_KEYDOWN && event->event.key.keysym.scancode == SDL_SCANCODE_F8) { // @suppress("Field cannot be resolved")
		_drawAxis = !_drawAxis;
	}





	// Handle events
	return true;//screen->eventHandler(event);
}

static cl_font_t* font;

static mat4 model_mat;
static std::vector<cl_VAO*> meshes;

void cl_r_makeProjection(Camera* camera) {
	mat4_frustum(&camera->projection, camera->aspect, camera->fov, camera->near, camera->far);
}

void cl_r_init() {

//	screen = new GuiScreen();

	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Starting renderer");
	rg_registerEventHandler(_r_handler);

	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "OpenGL Renderer: %s %s", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "OpenGL Version: %s; GLSL: %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_DEPTH_TEST);

	s_gbuffer = shader_create("platform/shader/g_main.vs", "platform/shader/g_main.fs", NULL);
	s_lpbuffer = shader_create("platform/shader/ds_lightpass.vs", "platform/shader/ds_lightpass.fs", NULL);
	s_combine = shader_create("platform/shader/ds_output.vs", "platform/shader/ds_output.fs", NULL);
	s_sslr = shader_create("platform/shader/ds_sslr.vs", "platform/shader/ds_sslr.fs", NULL);
	s_shadowqmap = shader_create("platform/shader/ds_shadowqmap.vs", "platform/shader/ds_shadowqmap.fs", "platform/shader/ds_shadowqmap.gs");


	s_particle = shader_create("platform/shader/fs_particle.vs", "platform/shader/fs_particle.fs", "platform/shader/fs_particle.gs");

	gbuffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 4, FBO_DEPTH);
	lp_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 2, 0);
	cb_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 2, 0);
	sslr_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 1, 0);


	s_dbg_light = shader_create("platform/shader/dbg_light.vs", "platform/shader/dbg_light.fs", NULL);
	dbg_light_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 1, 0);

	cl_skybox_init();
	cl_grass_init();
	cl_water_init();

	rg_mesh_t* mesh = rg_newMesh();
	// Yes, i know what i'm doing! //
	mesh->vertices = (rg_vertex_t*)vtx;
	mesh->indices = (Uint32*)inds;
	mesh->vertex_count = 4;
	mesh->index_count = 6;
	quad = cl_makeVAO(mesh);
	rg_freeMesh(mesh);

	camera.position.x = 0;
	camera.position.y = 3;
	camera.position.z = 3;

	camera.rotation.x = 0.0;
	camera.rotation.y = 0;
	camera.rotation.z = 0;

	camera.aspect = cl_display_getAspect();
	camera.fov = 75;
	camera.near = 0.1;
	camera.far = 3000;

	cl_r_makeProjection(&camera);

	cl_r2d_init();
	cl_rl3d_init();
	font = cl_font_new("platform/f_gui.ttf", 42);
	cl_gui_console_init();

	mat4_identity(&model_mat);

	glGenFramebuffers(4, shadow_mapFBO);
	glGenTextures(4, shadow_qubemaps);
	for (Uint32 i = 0; i < 4; ++i) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_qubemaps[i]);
		for (Uint32 j = 0; j < 6; ++j) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, shadow_mapFBO[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_qubemaps[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	glGenBuffers(1, &p_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cl_particle) * MAX_PARTICLES, NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cl_particle), (GLvoid*)(sizeof(float) * 0)); // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(cl_particle), (GLvoid*)(sizeof(float) * 3)); // size, time, rotation
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(cl_particle), (GLvoid*)(sizeof(float) * 6)); // velocity
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	cl_particle p0;
	cl_particle p1;
	cl_particle p2;
	SDL_memset(&p0, 0, sizeof(cl_particle));
	SDL_memset(&p1, 0, sizeof(cl_particle));
	SDL_memset(&p2, 0, sizeof(cl_particle));
	p0.y = 3;
	p1.y = 4;
	p2.y = 5;
	particles.push_back(p0);
	particles.push_back(p1);
	particles.push_back(p2);
//	particles.push_back({0, 2, 0,  0.3, 0, 0,  0, 0, 0});
//	particles.push_back({2, 0, 2,  0.3, 0, 0,  0, 0, 0});


	rg_Resource* res = rg_loadResource("gamedata/meshes/box.rgm");
	rg_mesh_t* mesht = rg_rmlConvert(res->data);
	dbg_light_box = cl_makeVAO(mesht);
	rg_freeMesh(mesht);
	rg_freeResource(res);

}

void cl_r_destroy() {
	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Shutting down...");

	cl_skybox_destroy();
	cl_grass_destroy();
	cl_water_destroy();
	shader_delete(s_gbuffer);
	shader_delete(s_lpbuffer);
	shader_delete(s_combine);
	shader_delete(s_shadowqmap);
	shader_delete(s_particle);
	shader_delete(s_sslr);

	glDeleteVertexArrays(1, &p_vao);
	glDeleteBuffers(1, &p_vbo);
	cl_fboFree(gbuffer);
	cl_fboFree(lp_buffer);
	cl_fboFree(cb_buffer);
	cl_fboFree(sslr_buffer);
	glDeleteFramebuffers(4, shadow_mapFBO);

	cl_fboFree(dbg_light_buffer);
	shader_delete(s_dbg_light);

	cl_gui_console_destroy();

	cl_r2d_destroy();
	cl_rl3d_destroy();
}


// TODO:
// GUI (panels, buttons, scroll bars, progress bars, etc)
// Implement in near future:
//
// SSLR reflections?
// Water


//
// server      engine        client
//  ----  << gameobject  => material
//


static void _r_draw2d(double dt) {
	cl_r2d_rotate({0, 0, 0});
	cl_r2d_translate({0, 0});

	float w = cl_display_getWidth(), h = cl_display_getHeight();

	if(allbright) {
		cl_r2d_bind(gbuffer->color[0]);
	} else {
//		cl_r2d_bind(cb_buffer->color[0]);
		cl_r2d_bind(sslr_buffer->color[0]);
	}
	cl_r2d_begin();
	cl_r2d_vertex({0, 0, 0, 0, 1, 1, 1, 1});
	cl_r2d_vertex({w, 0, 1, 0, 1, 1, 1, 1});
	cl_r2d_vertex({w, h, 1, 1, 1, 1, 1, 1});
	cl_r2d_vertex({w, h, 1, 1, 1, 1, 1, 1});
	cl_r2d_vertex({0, h, 0, 1, 1, 1, 1, 1});
	cl_r2d_vertex({0, 0, 0, 0, 1, 1, 1, 1});
	cl_r2d_end();

	cl_gui_draw(dt);

	cl_r2d_rotate({0, 0, 0});
	cl_r2d_translate({0, 0});
//	screen->drawScreen(dt);


	cl_r2d_rotate({0, 0, 0});
	cl_r2d_translate({5, cl_display_getHeight() - 50});

	cl_r2d_bind(0);
	cl_r2d_begin();
	cl_r2d_vertex({0,   -60,  0, 0, 0, 0, 0, 0.5});
	cl_r2d_vertex({140, -60,  1, 0, 0, 0, 0, 0.5});
	cl_r2d_vertex({140,  45, 1, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({140,  45, 1, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({0,    45, 0, 1, 0, 0, 0, 0.5});
	cl_r2d_vertex({0,   -60,  0, 0, 0, 0, 0, 0.5});
	cl_r2d_end();

	cl_r2d_translate({5, cl_display_getHeight() - 27});
	cl_r2d_drawString(font, L"Fps", 5, 5, 0.25, 1, 1, 1, 1);
	cl_r2d_drawString(font, std::to_wstring((int)rg_fps_avg).c_str(), 35, 5, 0.28, 1, 1, 1, 1);
	cl_r2d_drawString(font, L"Mem len", 5, -15, 0.25, 1, 1, 1, 1);
	cl_r2d_drawString(font, std::to_wstring(rg_getAllocatedMem()).c_str(), 55, -15, 0.28, 1, 1, 1, 1);

	cl_r2d_drawString(font, L"Camera:", 5, -30, 0.25, 1, 1, 1, 1);
	cl_r2d_drawString(font, std::to_wstring(camera.position.x).c_str(), 5, -45, 0.28, 1, 1, 1, 1);
	cl_r2d_drawString(font, std::to_wstring(camera.position.y).c_str(), 5, -60, 0.28, 1, 1, 1, 1);
	cl_r2d_drawString(font, std::to_wstring(camera.position.z).c_str(), 5, -75, 0.28, 1, 1, 1, 1);

	cl_gui_console_update(dt);
}

static void _r_drawaxis(float s) {
	cl_rl3d_begin();
	cl_rl3d_vertex({0, 0, 0, 1, 0, 0, 1});
	cl_rl3d_vertex({s, 0, 0, 1, 0, 0, 1});
	cl_rl3d_vertex({0, 0, 0, 0, 1, 0, 1});
	cl_rl3d_vertex({0, s, 0, 0, 1, 0, 1});
	cl_rl3d_vertex({0, 0, 0, 0, 0, 1, 1});
	cl_rl3d_vertex({0, 0, s, 0, 0, 1, 1});
	cl_rl3d_end();
}

static void _r_drawBone(GameObject* anim_obj, rg_Bone* bone) {
	vec4 p_new_pos;
	vec4 p_pos = {
		anim_obj->global_transforms[bone->id].m03,
		anim_obj->global_transforms[bone->id].m13,
		anim_obj->global_transforms[bone->id].m23,
		0
	};

	mat4_mul(&p_new_pos, &p_pos, &anim_obj->transform);

//	p_new_pos.x /= p_new_pos.w;
//	p_new_pos.y /= p_new_pos.w;
//	p_new_pos.z /= p_new_pos.w;

	for (size_t i = 0; i < bone->child_count; ++i) {
		rg_Bone* b = &bone->childs[i];
		vec4 new_pos;
		vec4 pos = {
			anim_obj->global_transforms[b->id].m03,
			anim_obj->global_transforms[b->id].m13,
			anim_obj->global_transforms[b->id].m23,
			0
		};

		mat4_mul(&new_pos, &pos, &anim_obj->transform);
//		new_pos.x /= new_pos.w;
//		new_pos.y /= new_pos.w;
//		new_pos.z /= new_pos.w;

//		float _x = anim_obj->global_transforms[b->id].m03;
//		float _y = anim_obj->global_transforms[b->id].m13;
//		float _z = anim_obj->global_transforms[b->id].m23;

		cl_rl3d_begin();
		cl_rl3d_vertex({p_new_pos.x, p_new_pos.y, p_new_pos.z, 1, 0, 0, 1});
		cl_rl3d_vertex({new_pos.x, new_pos.y, new_pos.z, 1, 0, 0, 1});
		cl_rl3d_end();
		_r_drawBone(anim_obj, b);
	}
}

static void _r_drawSkeleton(GameObject* anim_obj) {
//	_r_drawBone(anim_obj, &anim_obj->skel.root_bone);
}

static void _r_draw3d(double dt) {
	if(r_canRender) {
		if(_drawAxis) {
			_r_drawaxis(3);

			for (Uint32 i = 0; i < rg_level->objects.size(); ++i) {
				GameObject* obj = rg_level->objects[i];
				if(obj != NULL) {
					cl_rl3d_applyMatrix(&obj->transform);
					_r_drawaxis(1);
				}
			}
		}

		if(_drawAxis) {
			for (size_t i = 0; i < rg_level->objects.size(); ++i) {
				if(rg_level->objects[i]->type == RG_OBJECT_ANIMATED) {
					GameObject* anim_obj = rg_level->objects[i];

					_r_drawSkeleton(anim_obj);

					for (size_t b = 0; b < anim_obj->skel.bone_count; ++b) {
						mat4 mat;
						mat4_mul(&mat, &anim_obj->transform, &anim_obj->global_transforms[b]);
						cl_rl3d_applyMatrix(&mat);
						_r_drawaxis(5.5);
					}
				}
			}
		}
	}
}

cl_font_t* cl_r_getDefaultFont() {
	return font;
}

static RG_INLINE cl_VAO* _cl_r_getMesh(Uint32 id) {
	if(meshes.size() < id) {
		return NULL;
	}
	return meshes[id];
}

static rg_string luniform[4][4] = {
	{"lights[0].position", "lights[0].color", "lights[0].attenuation", "lights[0].radius"},
	{"lights[1].position", "lights[1].color", "lights[1].attenuation", "lights[1].radius"},
	{"lights[2].position", "lights[2].color", "lights[2].attenuation", "lights[2].radius"},
	{"lights[3].position", "lights[3].color", "lights[3].attenuation", "lights[3].radius"}
};

static void _cl_r_loadLight(rg_Shader shader, PointLight** light) {
	for (Uint32 i = 0; i < 4; ++i) {
		if(light[i] != NULL) {
			shader_uniform_3fp(shader_uniform_get(shader, luniform[i][0]), (float*)&light[i]->position);
			shader_uniform_3fp(shader_uniform_get(shader, luniform[i][1]), (float*)&light[i]->color);
			shader_uniform_3fp(shader_uniform_get(shader, luniform[i][2]), (float*)&light[i]->attenuation);
			shader_uniform_1f(shader_uniform_get(shader,  luniform[i][3]), light[i]->radius);
		} else {
			shader_uniform_1f(shader_uniform_get(shader,  luniform[i][3]), 0);
		}
	}
}

static RG_INLINE void _cl_r_beginLightShader() {
	shader_start(s_lpbuffer);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "diffuse"), 0);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "normal"), 1);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "vertex"), 2);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "prew"), 3);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "glow"), 4);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "lsmap[0]"), 5);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "lsmap[1]"), 6);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "lsmap[2]"), 7);
	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "lsmap[3]"), 8);

	shader_uniform_1i(shader_uniform_get(s_lpbuffer, "allowShadows"), _allowShadows);

	shader_uniform_3fp(shader_uniform_get(s_lpbuffer, "camera_pos"), (float*)&camera.position);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer->color[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer->color[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer->color[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lp_buffer->color[0]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gbuffer->color[3]);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_qubemaps[0]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_qubemaps[1]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_qubemaps[2]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_qubemaps[3]);
}

static void _cl_r_loadBoneOffset(rg_Shader shader, GameObject* obj, rg_Bone* bone) {
	char ou_name[128];
	strcpy(ou_name, "bonesOffsets[");
	std::stringstream ss; // mb rewrite this?
	ss << bone->id;
	strcat(ou_name, ss.str().c_str());
	strcat(ou_name, "]");
	shader_uniform_mat4f(shader_uniform_get(shader, ou_name), (float*)&bone->offset);

	for (size_t i = 0; i < bone->child_count; ++i) {
		_cl_r_loadBoneOffset(shader, obj, &bone->childs[i]);
	}
}

static void _cl_r_loadBoneMatrices(rg_Shader shader, GameObject* obj) {
	char mu_name[128];

	for (size_t i = 0; i < obj->skel.bone_count; ++i) {
		strcpy(mu_name, "bonesMatrices[");
		std::stringstream ss; // mb rewrite this?
		ss << i;
		strcat(mu_name, ss.str().c_str());
		strcat(mu_name, "]");
		shader_uniform_mat4f(shader_uniform_get(shader, mu_name), (float*)&obj->global_transforms[i]);
	}

	_cl_r_loadBoneOffset(shader, obj, &obj->skel.root_bone);
}

static void _cl_r_calcShadowQmaps(PointLight** light) {
	if(!_allowShadows)
		return;

	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	for (Uint32 i = 0; i < 4; ++i) {
		if(light[i] == NULL) { continue; }
		mat4 proj;
		mat4 view[6];
		mat4_frustum(&proj, 1, 90, 1, light[i]->radius);
		float d90 = PI/2;
		float d180 = PI;
		mat4_viewZ(&view[0], light[i]->position.x, light[i]->position.y, light[i]->position.z,  d90, 0,    d180);
		mat4_viewZ(&view[1], light[i]->position.x, light[i]->position.y, light[i]->position.z, -d90, 0,    d180);
		mat4_viewZ(&view[2], light[i]->position.x, light[i]->position.y, light[i]->position.z, d180,  d90, d180);
		mat4_viewZ(&view[3], light[i]->position.x, light[i]->position.y, light[i]->position.z, d180, -d90, d180);
		mat4_viewZ(&view[4], light[i]->position.x, light[i]->position.y, light[i]->position.z, d180, 0,    d180);
		mat4_viewZ(&view[5], light[i]->position.x, light[i]->position.y, light[i]->position.z,    0, 0,    d180);

		glBindFramebuffer(GL_FRAMEBUFFER, shadow_mapFBO[i]);
		glClear(GL_DEPTH_BUFFER_BIT);
		shader_start(s_shadowqmap);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[0]"), (float*)&view[0]);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[1]"), (float*)&view[1]);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[2]"), (float*)&view[2]);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[3]"), (float*)&view[3]);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[4]"), (float*)&view[4]);
		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "matrices[5]"), (float*)&view[5]);

		shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "proj"), (float*)&proj);
		shader_uniform_3fp(shader_uniform_get(s_shadowqmap, "lightPos"), (float*)&light[i]->position);
		shader_uniform_1f(shader_uniform_get(s_shadowqmap, "far_plane"), light[i]->radius);
		for (Uint32 i = 0; i < rg_level->objects.size(); ++i) {
			GameObject* obj = rg_level->objects[i];
			shader_uniform_mat4f(shader_uniform_get(s_shadowqmap, "model"), (float*)&obj->transform);
			cl_VAO* mesh = _cl_r_getMesh(obj->mesh_id);

			SDL_assert_always(mesh);

			if(!mesh->anim) {
				shader_uniform_1i(shader_uniform_get(s_shadowqmap, "anim"), 0);
			} else {
				if(_doAnimation) {
					shader_uniform_1i(shader_uniform_get(s_shadowqmap, "anim"), 1);
					_cl_r_loadBoneMatrices(s_shadowqmap, obj);
				}
			}
			cl_drawVAO(mesh);
		}
	}
	glViewport(0, 0, cl_display_getWidth(), cl_display_getHeight());
}

void cl_r_doRender(double dt) {
	double _time = rg_getRunningTime();

	cl_display_setMouseGrabbed(g_cursor);

	cl_camera_recalcViewMatrix(&camera);
	if(_changeViewport) {
		_changeViewport = false;
		glViewport(0, 0, (int)cl_display_getWidth(), (int)cl_display_getHeight());
		// Update framebuffers

		// TODO: Make fbo function!!!

		cl_fboFree(gbuffer);
		cl_fboFree(lp_buffer);
		cl_fboFree(sslr_buffer);
		cl_fboFree(dbg_light_buffer);
		gbuffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 4, FBO_DEPTH);
		lp_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 2, 0);
		cb_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 2, 0);
		sslr_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 1, 0);
		dbg_light_buffer = cl_fboNew(cl_display_getWidth(), cl_display_getHeight(), 1, 0);
	}

	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);

//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	//                               GEOMETRY  PASS                               //
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	cl_fboBind(gbuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear gbuffer


	shader_start(s_gbuffer);
	shader_uniform_mat4f(shader_uniform_get(s_gbuffer, "proj"), (float*)&camera.projection);
	shader_uniform_mat4f(shader_uniform_get(s_gbuffer, "view"), (float*)&camera.view);

	shader_uniform_1i(shader_uniform_get(s_gbuffer, "diffuse"), 0);
	shader_uniform_1i(shader_uniform_get(s_gbuffer, "normal"), 1);
	shader_uniform_1i(shader_uniform_get(s_gbuffer, "roughness"), 2);
	shader_uniform_1i(shader_uniform_get(s_gbuffer, "metallic"), 3);
	shader_uniform_1i(shader_uniform_get(s_gbuffer, "glow"), 4);
	shader_uniform_1f(shader_uniform_get(s_gbuffer, "tiling"), 1);
	shader_uniform_1f(shader_uniform_get(s_gbuffer, "time"), _time);
	shader_uniform_3fp(shader_uniform_get(s_gbuffer, "cam_pos"), (float*)&camera.position);


	// Render objects
	if(r_canRender) {
		cl_skybox_render(s_gbuffer);
		cl_grass_render(dt, s_gbuffer);
		cl_water_render(dt, s_gbuffer);

		shader_uniform_1i(shader_uniform_get(s_gbuffer, "surface_type"), SURFACE_DEFAULT);
//		SDL_Log("Objects to render: %ld", rg_level->objects.size());
		for (Uint32 i = 0; i < rg_level->objects.size(); ++i) {
			GameObject* obj = rg_level->objects[i];

			if(obj != NULL) {
				shader_uniform_mat4f(shader_uniform_get(s_gbuffer, "model"), (float*)&obj->transform);
				rg_material_t* mat = cl_mat_get(obj->mat_id);
				if(mat != NULL) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mat->diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, mat->normal);
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, mat->roughness);
					glActiveTexture(GL_TEXTURE3);
					glBindTexture(GL_TEXTURE_2D, mat->metallic);
					glActiveTexture(GL_TEXTURE4);
					glBindTexture(GL_TEXTURE_2D, mat->glow);
					cl_VAO* mesh = _cl_r_getMesh(obj->mesh_id);
					if(mesh != NULL) {
						if(!mesh->anim) {
							shader_uniform_1i(shader_uniform_get(s_gbuffer, "anim"), 0);
						} else {
							if(_doAnimation) {
								shader_uniform_1i(shader_uniform_get(s_gbuffer, "anim"), 1);
								_cl_r_loadBoneMatrices(s_gbuffer, obj);
							}
						}
						cl_drawVAO(mesh);
					}
				}
			}
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	//                             SCREEN-SPACE  PASS                             //
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

//	if(rg_level != NULL) {


	if(_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(r_canRender) {
//		rg_level->lights[0].position.x = SDL_cos(_time*0.8)*4;
//		rg_level->lights[0].position.z = SDL_sin(_time*0.8)*4;
//		rg_level->lights[0].position.y = 4 + SDL_sin(_time*2)*0.7;
//
//		rg_level->lights[1].position.x = SDL_cos(PI/2 + _time*0.8)*4;
//		rg_level->lights[1].position.z = SDL_sin(PI/2 + _time*0.8)*4;
//		rg_level->lights[1].position.y = 4 + SDL_sin(PI/2 + _time*2)*0.7;

		rg_level->lights[0].position.x = SDL_cos(_time*0.8)*6;
		rg_level->lights[0].position.z = 4;
		rg_level->lights[0].position.y = 4 + SDL_sin(_time*2)*0.2;

		rg_level->lights[1].position.x = SDL_cos(PI/2 + _time*0.8)*6;
		rg_level->lights[1].position.z = 4;
		rg_level->lights[1].position.y = 4 + SDL_sin(PI/2 + _time*2)*0.2;


		// Light
		cl_fboBind(lp_buffer);
		glClear(GL_COLOR_BUFFER_BIT);

		PointLight* plight[4];
		Uint32 i = 0;
		for (; i < rg_level->lights.size() / 4; ++i) {
			plight[0] = &rg_level->lights[i*4];
			plight[1] = &rg_level->lights[i*4 + 1];
			plight[2] = &rg_level->lights[i*4 + 2];
			plight[3] = &rg_level->lights[i*4 + 3];
			_cl_r_calcShadowQmaps(plight);
			cl_fboBind(lp_buffer);
			_cl_r_beginLightShader();
			_cl_r_loadLight(s_lpbuffer, plight);
			cl_drawVAO(quad);
		}

		if(rg_level->lights.size() % 4 != 0) {
			plight[0] = plight[1] = plight[2] = plight[3] = NULL;
			for (Uint32 j = 0; j < rg_level->lights.size() % 4; ++j) {
				plight[j] = &rg_level->lights[i*4 + j];
			}

			_cl_r_calcShadowQmaps(plight);
			cl_fboBind(lp_buffer);
			_cl_r_beginLightShader();
			_cl_r_loadLight(s_lpbuffer, plight);
			cl_drawVAO(quad);
		}

		// Draw point light
		cl_fboBind(dbg_light_buffer);
		glClear(GL_COLOR_BUFFER_BIT);
		shader_start(s_dbg_light);
		shader_uniform_mat4f(shader_uniform_get(s_dbg_light, "proj"), (float*)&camera.projection);
		shader_uniform_mat4f(shader_uniform_get(s_dbg_light, "view"), (float*)&camera.view);

		for (size_t i = 0; i < rg_level->lights.size(); ++i) {
			mat4_model(&model_mat, rg_level->lights[i].position.x, rg_level->lights[i].position.y, rg_level->lights[i].position.z, 0, 0, 0, 0.1);
			shader_uniform_mat4f(shader_uniform_get(s_dbg_light, "model"), (float*)&model_mat);
			shader_uniform_3fp(shader_uniform_get(s_dbg_light, "color"), (float*)&rg_level->lights[i].color);
			cl_drawVAO(dbg_light_box);
		}

		// Combine
		cl_fboBind(cb_buffer);
		glClear(GL_COLOR_BUFFER_BIT);
		shader_start(s_combine);

		shader_uniform_1i(shader_uniform_get(s_combine, "diffuse"), 0);
		shader_uniform_1i(shader_uniform_get(s_combine, "lightmap"), 1);
		shader_uniform_1i(shader_uniform_get(s_combine, "bloom"), 2);
		shader_uniform_1i(shader_uniform_get(s_combine, "dbg_lights"), 3);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer->color[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, lp_buffer->color[0]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, lp_buffer->color[1]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, dbg_light_buffer->color[0]);
		cl_drawVAO(quad);

		// Screen-space local reflections
		cl_fboBind(sslr_buffer);
		glClear(GL_COLOR_BUFFER_BIT);
		shader_start(s_sslr);
		shader_uniform_mat4f(shader_uniform_get(s_sslr, "proj"), (float*)&camera.projection);
		shader_uniform_3fp(shader_uniform_get(s_sslr, "cam_pos"), (float*)&camera.position);
		shader_uniform_mat4f(shader_uniform_get(s_sslr, "view"), (float*)&camera.view);

		shader_uniform_1i(shader_uniform_get(s_sslr, "diffuse"), 0);
		shader_uniform_1i(shader_uniform_get(s_sslr, "normal"), 1);
		shader_uniform_1i(shader_uniform_get(s_sslr, "vertex"), 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cb_buffer->color[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gbuffer->color[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer->color[2]);
		cl_drawVAO(quad);
	}



	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
	//                                  GUI PASS                                  //
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cl_fboBind(0);
	glClearColor(0.3, 0.5, 0.9, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear default framebuffer

	cl_r2d_doRender(dt, _r_draw2d);

	glDisable(GL_CULL_FACE);
	cl_rl3d_doRender(dt, _r_draw3d, &camera);

	glEnable(GL_DEPTH_TEST);



//	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//	//                                  --------                                  //
//	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//	glDisable(GL_CULL_FACE);
//
//	shader_start(s_particle);
//	shader_uniform_mat4f(shader_uniform_get(s_particle, "proj"), (float*)&camera.projection);
//	shader_uniform_mat4f(shader_uniform_get(s_particle, "view"), (float*)&camera.view);
//
//	//cl_drawVAO(quad);
//
//	glBindBuffer(GL_ARRAY_BUFFER, p_vao);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cl_particle) * particles.size(), &particles[0]);
////	SDL_memcpy(_particles, &particles[0], sizeof(cl_particle) * particles.size());
////	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_particles), _particles);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, gbuffer->color[1]);
//	glDrawArrays(GL_POINTS, 0, particles.size());

//	SDL_Log("Particles: %d", particles.size());

//	SDL_Log("GL: %s", glewGetErrorString(glGetError()));
}

void cl_r_canRender(bool cr) {
	r_canRender = cr;
}

void cl_r_unloadMeshes() {
//	rg_print("unloading meshes: %d", meshes.size());
	for (Uint32 i = 0; i < meshes.size(); ++i) {
//		rg_print("%d -> c %d", i, meshes[i]->i_count);
		cl_deleteVAO(meshes[i]);
	}
	meshes.clear();
//	rg_print("done!");
}

void cl_r_loadMesh(rg_mesh mesh) {
	char path[128];
	rg_buildResourcePath(mesh.name, path, "meshes");

	SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "Load mesh: %s", path);

	rg_Resource* res = rg_loadResource(path);
	if(mesh.isAnimated) {
		AnimatedMesh* amesh = rg_armlConvert(res->data);
		cl_VAO* vao = cl_makeAVAO(amesh);
		meshes.push_back(vao);
		rg_free(amesh);
	} else {
		rg_mesh_t* mesht = rg_rmlConvert(res->data);
		cl_VAO* vao = cl_makeVAO(mesht);
		meshes.push_back(vao);
		rg_freeMesh(mesht);
	}

	rg_freeResource(res);
}

Camera* cl_r_getCamera() {
	return &camera;
}
