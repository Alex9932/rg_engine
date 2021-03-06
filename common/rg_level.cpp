/*
 * rg_level.cpp
 *
 *  Created on: 11 апр. 2021 г.
 *      Author: alex9932
 */

// PHYSICS

/*
"physics" section
 {
 	 "type": [COLLIDER (AABB => 0, SPHERE => 1, ...), IS KINEMATIC (0, 1)]
 	 "COLLIDER (aabb, sphere, ...)": []
 	 "entity": ENTITY ID (-1 => not linked to entity)
 }

*/

// TODO: !!! REWRITE THIS !!!

#include "rg_level.h"
#include "rg_string.h"
#include "rg_loader.h"

#include <SDL2/SDL.h>
#include <string.h>

#include "rg_animator.h"

Level* rg_newLevel() {
	return new Level;
}

void rg_freeLevel(Level* level) {
	rg_phys_clearWorld();
	cJSON_Delete(level->config);
	rg_freeResource(level->level_res);
//	for (Uint32 i = 0; i < level->materials.size(); ++i) {
//		rg_free(level->materials[i]);
//	}
//	for (Uint32 i = 0; i < level->meshes.size(); ++i) {
//		rg_free(level->meshes[i]);
//	}
	for (Uint32 i = 0; i < level->objects.size(); ++i) {
		rg_freeGameobject(level->objects[i]);
	}
	delete level;
}

GameObject* rg_newGameobject() {
	GameObject* obj = (GameObject*)rg_malloc(sizeof(GameObject));
	SDL_memset(obj, 0, sizeof(GameObject));
	obj->scale = {1, 1, 1};
	return obj;
}

static void _rg_freeBone(rg_Bone* bone) {
	for (size_t i = 0; i < bone->child_count; ++i) {
		_rg_freeBone(&bone->childs[i]);
	}
	if(bone->child_count != 0) {
		rg_free(bone->childs);
	}
}

void rg_freeGameobject(GameObject* ptr) {
	if(ptr->type == RG_OBJECT_ANIMATED) {
		rg_free(ptr->local_transforms);
		rg_free(ptr->global_transforms);
		_rg_freeBone(&ptr->skel.root_bone);

		// TODO: REMOVE THIS !!! (temporary solution)
		rg_freeAnimation(ptr->animation_current);
	}
	rg_free(ptr);
}

struct _Bone {
	Sint32 id;
	std::vector<_Bone> childs;
};

static void _rg_findChilds(AnimatedMesh* am, _Bone* parent) {
	for (size_t i = 0; i < am->skeleton.bone_count; ++i) {
		if(am->skeleton.bones[i].parent_id == parent->id) {
			_Bone bone;
			bone.id = am->skeleton.bones[i].id;
			parent->childs.push_back(bone);
			_rg_findChilds(am, &parent->childs[parent->childs.size()-1]);
		}
	}
}

static void _rg_convertSkeleton(GameObject* obj, AnimatedMesh* am, rg_Bone* bone, _Bone* parent, mat4* parentBindTransform) {
	bone->id = parent->id;
	bone->child_count = parent->childs.size();
	Bone b;
	for (size_t i = 0; i < am->skeleton.bone_count; ++i) {
		if(am->skeleton.bones[i].id == bone->id) { b = am->skeleton.bones[i]; break; }
	}
	mat4 bind_matrix;
	bone->offset = b.matrix;
	if(bone->child_count == 0) { return; }
	bone->childs = (rg_Bone*)rg_malloc(sizeof(rg_Bone) * bone->child_count);
	for (size_t i = 0; i < bone->child_count; ++i) {
		_rg_convertSkeleton(obj, am, &bone->childs[i], &parent->childs[i], &bind_matrix);
	}
}

static void _rg_processAnimation(GameObject* obj, AnimatedMesh* am) {
	obj->skel.bone_count = am->skeleton.bone_count;
	obj->global_transforms = (mat4*)rg_malloc(sizeof(mat4) * obj->skel.bone_count);
	obj->local_transforms = (mat4*)rg_malloc(sizeof(mat4) * obj->skel.bone_count);
	for (size_t m = 0; m < obj->skel.bone_count; ++m) {
		mat4_identity(&obj->global_transforms[m]);
	}

	_Bone root;
	// Find Root bone
	for (size_t i = 0; i < am->skeleton.bone_count; ++i) {
		if(am->skeleton.bones[i].parent_id == -1) {
			root.id = am->skeleton.bones[i].id;
			break;
		}
	}

	_rg_findChilds(am, &root);
	mat4 IDENTITY;
	mat4_identity(&IDENTITY);
	_rg_convertSkeleton(obj, am, &obj->skel.root_bone, &root, &IDENTITY);
}

int rg_loadLevel(Level* level, rg_string level_name) {
	memset(level->levelname, '\0', 64);
	memcpy(level->levelname, level_name, strlen(level_name));
	char path[128];
	memset(path, '\0', 128);
	strcpy(path, "gamedata/levels/");
	strcat(path, level_name);
	strcat(path, "/");

	SDL_Log("Loading level: %s", path);

	char path_leveljson[128];
	memset(path_leveljson, '\0', 128);
	strcpy(path_leveljson, path);
	strcat(path_leveljson, "level.json");

	char path_levelscript[128];
	memset(path_levelscript, '\0', 128);
	strcpy(path_levelscript, path);
	strcat(path_levelscript, "level.js");

	level->js_state = rg_js_makeDefaultState();
	rg_js_execute(&level->js_state, path_levelscript);

	level->level_res = rg_loadResource(path_leveljson);
	level->config = cJSON_Parse((rg_string)level->level_res->data);
	rg_string error = cJSON_GetErrorPtr();
	if(error != NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Level parsing error!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "At: %s", error);
		return -1;
	}

	cJSON* _meshes    = cJSON_GetObjectItem(level->config, "meshes");
	cJSON* _materials = cJSON_GetObjectItem(level->config, "materials");
	cJSON* _entitys   = cJSON_GetObjectItem(level->config, "entitys");
	cJSON* _terrains  = cJSON_GetObjectItem(level->config, "terrains");
	cJSON* _lights    = cJSON_GetObjectItem(level->config, "lights");
	cJSON* _physics   = cJSON_GetObjectItem(level->config, "physics");
	int a = cJSON_GetArraySize(_meshes);
	int b = cJSON_GetArraySize(_materials);
	int c = cJSON_GetArraySize(_entitys);
	int d = cJSON_GetArraySize(_terrains);
	int e = cJSON_GetArraySize(_lights);
	int f = cJSON_GetArraySize(_physics);

	for (int i = 0; i < a; ++i) {
		cJSON* _m = cJSON_GetArrayItem(_meshes, i);
		rg_mesh mesh;
		mesh.name = cJSON_GetStringValue(cJSON_GetArrayItem(_m, 0));
		mesh.isAnimated = cJSON_GetNumberValue(cJSON_GetArrayItem(_m, 1));
		level->meshes.push_back(mesh);
	}

	for (int i = 0; i < b; ++i) {
//		char* s = cJSON_GetStringValue(cJSON_GetArrayItem(_materials, i));
//		SDL_Log("level: mat -> %s", s);
//		level->materials.push_back(s);
		level->materials.push_back(cJSON_GetStringValue(cJSON_GetArrayItem(_materials, i)));
	}

	for (int i = 0; i < c; ++i) {
		cJSON* _entity = cJSON_GetArrayItem(_entitys, i);
		GameObject* obj = rg_newGameobject();
		cJSON* __pos = cJSON_GetObjectItem(_entity, "position");
		obj->position.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 0));
		obj->position.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 1));
		obj->position.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 2));
		cJSON* __rot = cJSON_GetObjectItem(_entity, "rotation");
		obj->rotation.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 0));
		obj->rotation.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 1));
		obj->rotation.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 2));
		obj->mesh_id = (Uint32)cJSON_GetNumberValue(cJSON_GetObjectItem(_entity, "mesh"));
		obj->mat_id = (Uint32)cJSON_GetNumberValue(cJSON_GetObjectItem(_entity, "material"));

//		obj->phys_body = rg_phys_createBox(
//				obj->position.x, obj->position.y, obj->position.z,
//				2, 2, 2,
//				5);

		if(level->meshes[obj->mesh_id].isAnimated) {
			char path[128];
			rg_buildResourcePath(level->meshes[obj->mesh_id].name, path, "meshes");
			rg_Resource* mres = rg_loadResource(path);
			AnimatedMesh* am = rg_armlConvert(mres->data);
			_rg_processAnimation(obj, am);
			rg_free(am);
			rg_freeResource(mres);

			obj->type = RG_OBJECT_ANIMATED;

			obj->animation_current = rg_loadAnimation("gamedata/anims/vampire.anim");
		} else {
			obj->skel.bone_count = 0;
			obj->type = RG_OBJECT_STATIC;
		}
		level->objects.push_back(obj);
	}

	for (int i = 0; i < d; ++i) {
		cJSON* _terrain = cJSON_GetArrayItem(_terrains, i);
		GameObject* obj = rg_newGameobject();
		cJSON* __pos = cJSON_GetObjectItem(_terrain, "position");
		obj->position.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 0));
		obj->position.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 1));
		obj->position.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 2));
		cJSON* __rot = cJSON_GetObjectItem(_terrain, "rotation");
		obj->rotation.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 0));
		obj->rotation.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 1));
		obj->rotation.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__rot, 2));
		obj->mesh_id = (Uint32)cJSON_GetNumberValue(cJSON_GetObjectItem(_terrain, "mesh"));
		obj->mat_id = (Uint32)cJSON_GetNumberValue(cJSON_GetObjectItem(_terrain, "material"));
		obj->type = RG_OBJECT_TERRAIN;
		level->objects.push_back(obj);
	}

//	SDL_Log("level: light");
	for (int i = 0; i < e; ++i) {
		cJSON* _light = cJSON_GetArrayItem(_lights, i);
		PointLight light;
		cJSON* __pos = cJSON_GetObjectItem(_light, "position");
		light.position.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 0));
		light.position.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 1));
		light.position.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__pos, 2));
		cJSON* __color = cJSON_GetObjectItem(_light, "color");
		light.color.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__color, 0));
		light.color.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__color, 1));
		light.color.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__color, 2));
		cJSON* __atten = cJSON_GetObjectItem(_light, "attenuation");
		light.attenuation.x = cJSON_GetNumberValue(cJSON_GetArrayItem(__atten, 0));
		light.attenuation.y = cJSON_GetNumberValue(cJSON_GetArrayItem(__atten, 1));
		light.attenuation.z = cJSON_GetNumberValue(cJSON_GetArrayItem(__atten, 2));

		float lightMax = std::fmaxf(std::fmaxf(light.color.x, light.color.y), light.color.z);
		light.radius   = (-light.attenuation.y +  SDL_sqrtf(light.attenuation.y * light.attenuation.y - 4 * light.attenuation.z * (light.attenuation.x - (256.0 / 5.0) * lightMax))) / (2 * light.attenuation.z);

		light.type = RG_LIGHTSOURCE_STATIC;
		level->lights.push_back(light);
	}

	for (int i = 0; i < f; ++i) {
		cJSON* _p_object = cJSON_GetArrayItem(_physics, i);

		cJSON* __type = cJSON_GetObjectItem(_p_object, "type");
		Uint32 collider     = (Uint32)cJSON_GetNumberValue(cJSON_GetArrayItem(__type, 0));
		Uint32 is_kinematic = (Uint32)cJSON_GetNumberValue(cJSON_GetArrayItem(__type, 1));
		Sint32 entity_id    = (Sint32)cJSON_GetNumberValue(cJSON_GetObjectItem(_p_object, "entity"));

		// Only AABB and SHPERE implemented
		rg_phys_object* phys_body = NULL;
		float x;
		float y;
		float z;

		switch (collider) {
			case RG_COLLIDER_AABB: {
				cJSON* __aabb = cJSON_GetObjectItem(_p_object, "aabb");
				x = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 0));
				y = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 1));
				z = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 2));
				float w = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 3));
				float h = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 4));
				float d = cJSON_GetNumberValue(cJSON_GetArrayItem(__aabb, 5));
//				SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Body: [%f, %f, %f, %f, %f, %f]", x, y, z, w, h, d);
				phys_body = rg_phys_createBox(x, y, z, w, h, d, 5);

				break;
			}
			case RG_COLLIDER_SPHERE: {
				cJSON* __sph = cJSON_GetObjectItem(_p_object, "sphere");
				x = cJSON_GetNumberValue(cJSON_GetArrayItem(__sph, 0));
				y = cJSON_GetNumberValue(cJSON_GetArrayItem(__sph, 1));
				z = cJSON_GetNumberValue(cJSON_GetArrayItem(__sph, 2));
				float r = cJSON_GetNumberValue(cJSON_GetArrayItem(__sph, 3));
//				phys_body = rg_phys_createBox(x, y, z, w, h, d, 5);
				phys_body = rg_phys_createSphere(x, y, z, r, 5);
				break;
			}
			default:
				rg_assert_msg(NULL, "Level loading error (UNKNOWN COLLIDER TYPE)");
				break;
		}

		if(is_kinematic) {
			SDL_LogInfo(SDL_LOG_CATEGORY_PHYSICS, "Kinematic object at: %f, %f, %f", x, y, z);
			dBodySetKinematic(phys_body->body);
		}

		if(entity_id != -1) {
			level->objects[entity_id]->phys_body = phys_body;
		}
	}

	rg_Event loadlevel_event;
	loadlevel_event.type = RG_EVENT_LOADLEVEL;
	rg_pushEvent(&loadlevel_event);
	return 0;
}

void rg_updateLevel(Level* level, double dt) {
	for (size_t i = 0; i < level->objects.size(); ++i) {
		GameObject* obj = level->objects[i];

		if(obj->phys_body == NULL) {
			mat4 scale_matrix;
			mat4 model_matrix;
			mat4_identity(&scale_matrix, &obj->scale);

			mat4_model(&model_matrix,
					obj->position.x, obj->position.y, obj->position.z,
					obj->rotation.x, obj->rotation.y, obj->rotation.z);

			mat4_mul(&obj->transform, &model_matrix, &scale_matrix);
		} else {
			rg_phys_getMatrix(&obj->transform, obj->phys_body->body);
			rg_phys_getPosition(&obj->position, obj->phys_body->body);
		}

		if(obj->type == RG_OBJECT_ANIMATED) {
			obj->scale.x = 0.03f;
			obj->scale.y = 0.03f;
			obj->scale.z = 0.03f;
			rg_updateAnimation(dt, obj);
		}
	}
}
