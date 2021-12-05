/*
 * rg_animation.cpp
 *
 *  Created on: Sep 28, 2021
 *      Author: alex9932
 */

#include "rg_animator.h"

mat4 IDENTITY_MATRIX = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

static void _rg_calcBoneTransform(GameObject* obj, rg_Bone* bone, mat4* parent_transform) {
	mat4 global_transform;
	mat4_mul(&global_transform, parent_transform, &obj->local_transforms[bone->id]);
	for (size_t i = 0; i < bone->child_count; ++i) {
		_rg_calcBoneTransform(obj, &bone->childs[i], &global_transform);
	}
	obj->global_transforms[bone->id] = global_transform;
}

void rg_updateAnimation(double dt, GameObject* obj) {
	Animation* anim = obj->animation_current;

	obj->animation_time += dt;
	int f0 = 0;
	for (size_t index = 0; index < anim->frame_count - 1; ++index) {
		if (obj->animation_time < anim->frames[index + 1].time_stamp) {
			f0 = index;
			break;
		}
	}
	int f1 = f0 + 1;
	if(obj->animation_time >= anim->frames[f1].time_stamp) {
		obj->animation_time = 0;
	}

	KeyFrame frame0 = anim->frames[f0];
	KeyFrame frame1 = anim->frames[f1];
	double midWayLength = obj->animation_time - frame0.time_stamp;
	double framesDiff = frame1.time_stamp - frame0.time_stamp;
	double anim_delta = midWayLength / framesDiff;

	for (size_t i = 0; i < anim->bone_count; ++i) {
		BoneTransform transform0 = frame0.bones[i];
		BoneTransform transform1 = frame1.bones[i];
		BoneTransform transform;

		if(transform0.id == -1) {
			continue;
		}

		mat4 translate;
		mat4 rotate;
		mat4 scale;
		mat4_identity(&translate);
		mat4_identity(&rotate);
		mat4_identity(&scale, &transform.scale);

		// Interpolate
		vec3_lerp(&transform.scale, &transform0.scale, &transform1.scale, anim_delta);
		vec3_lerp(&transform.position, &transform0.position, &transform1.position, anim_delta);
		quat_slerp(&transform.orientation, &transform0.orientation, &transform1.orientation, anim_delta);
		mat4_translate(&translate, transform.position.x, transform.position.y, transform.position.z);
		mat4_quat(&rotate, &transform.orientation);
		mat4_mul(&obj->local_transforms[transform0.id], &translate, &rotate);
	}
	_rg_calcBoneTransform(obj, &obj->skel.root_bone, &IDENTITY_MATRIX);
}
