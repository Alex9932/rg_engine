/*
 * rg_anim.h
 *
 *  Created on: Oct 4, 2021
 *      Author: alex9932
 */

#ifndef RG_ANIM_H_
#define RG_ANIM_H_

#include <SDL2/SDL.h>
#include "rg_vecmath.h"
#include "rg_mesh.h"

typedef struct Weights {
	Sint32 bone_ids[4];
	float weights[4];
} Weights;

typedef struct Bone {
	Uint32 id;
	Sint32 parent_id;
	mat4 matrix; // Offset
} Bone;

typedef struct BoneTransform {
	Sint32 id;
	vec3 position;
	vec3 scale;
	quat orientation;
} BoneTransform;

typedef struct KeyFrame {
	double time_stamp;
	BoneTransform* bones;
} KeyFrame;

typedef struct Animation {
	Uint32 bone_count;
	Uint32 frame_count;
	Uint32 tps;
	double duration;
	KeyFrame* frames;
} Animation;

typedef struct Skeleton {
	Bone* bones;
	Uint32 bone_count;
} Skeleton;

typedef struct AnimatedMesh {
	rg_mesh_t mesh;
	Weights* weights;
	Skeleton skeleton;
} AnimatedMesh;

// ~~~~~~~~~~~~~~~~~~~~~~~~ //

typedef struct rg_Bone {
	Uint32 id;
	Uint32 child_count;
	rg_Bone* childs;
	mat4 offset;
} rg_Bone;

typedef struct rg_Skeleton {
	rg_Bone root_bone;
	Uint32 bone_count;
} rg_Skeleton;

typedef struct rg_AnimatedMesh {
	rg_mesh_t mesh;
	Weights* weights;
	rg_Skeleton skeleton;
} rg_AnimatedMesh;

#endif /* RG_ANIM_H_ */
