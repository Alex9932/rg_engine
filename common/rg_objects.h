/*
 * rg_objects.h
 *
 *  Created on: Dec 5, 2021
 *      Author: alex9932
 */

#ifndef RG_OBJECTS_H_
#define RG_OBJECTS_H_

#include "rg_vecmath.h"
#include "rg_physics.h"
#include "rg_animation.h"

enum LightSourceType {
	RG_LIGHTSOURCE_STATIC  = 0, // Calculate light's radius once
	RG_LIGHTSOURCE_DYNAMIC = 1  // Calculate light's radius every frame
};

enum ObjectType {
	RG_OBJECT_STATIC = 0,
	RG_OBJECT_DYNAMIC = 1,
	RG_OBJECT_TERRAIN = 2,
	RG_OBJECT_ANIMATED = 3
};

// Structs

typedef struct PointLight {
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float radius;
	LightSourceType type;
} PointLight;

typedef struct GameObject {
	vec3 position;
	vec3 rotation;
	vec3 scale;
	mat4 transform;
	Uint32 mat_id;
	Uint32 mesh_id;
	ObjectType type;
	rg_phys_object* phys_body;

	// Only for animated objects
	rg_Skeleton skel;
	mat4* local_transforms; // Local bone transforms
	mat4* global_transforms; // Global transforms (to shader)

	double animation_time;
	Animation* animation_current;
} GameObject;

#endif /* RG_OBJECTS_H_ */
