/*
 * rg_level.h
 *
 *  Created on: 11 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_LEVEL_H_
#define RG_LEVEL_H_

#include "rg_engine.h"
#include "rg_vecmath.h"
#include "rg_anim.h"
#include <vector>
#include <cJSON.h>

enum cl_LightType {
	RG_LIGHT_STATIC  = 0, // Calculate light's radius once
	RG_LIGHT_DYNAMIC = 1  // Calculate light's radius every frame
};

typedef struct cl_PointLight {
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float radius;
	cl_LightType type;
} cl_PointLight;

enum rg_object_type {
	RG_OBJECT_STATIC = 0,
	RG_OBJECT_DYNAMIC = 1,
	RG_OBJECT_TERRAIN = 2,
	RG_OBJECT_ANIMATED = 3
};

typedef struct rg_mesh {
	char* name;
	bool isAnimated;
} rg_mesh;

typedef struct rg_object_t {
	vec3 position;
	vec3 rotation;
	Uint32 mat_id;
	Uint32 mesh_id;
	rg_object_type type;

	// Only for animated objects
	rg_Skeleton skel;
	mat4* local_transforms; // Local bone transforms
	mat4* global_transforms; // Global transforms (to shader)
} rg_object_t;

typedef struct rg_level_t {
	Uint8 isClient;
	std::vector<rg_object_t*> objects;
	std::vector<cl_PointLight> lights;
	std::vector<char*> materials;
	std::vector<rg_mesh> meshes;
	cJSON* config;
	char levelname[64];
	rg_Resource* level_res;
} rg_level_t;

rg_level_t* rg_newLevel();
void rg_freeLevel(rg_level_t* level);
int rg_loadLevel(rg_level_t* level, rg_string level_name);
rg_object_t* rg_newGameobject();
void rg_freeGameobject(rg_object_t* ptr);

#endif /* RG_LEVEL_H_ */
