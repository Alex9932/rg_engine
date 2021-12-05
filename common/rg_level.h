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
#include "rg_loader.h"
#include "rg_script.h"
#include <vector>
#include <cJSON.h>
#include <rg_animation.h>
#include "rg_objects.h"

typedef struct rg_mesh {
	char* name;
	bool isAnimated;
} rg_mesh;

typedef struct Level {
	Uint8 isClient;
	std::vector<GameObject*> objects;
	std::vector<PointLight> lights;
	std::vector<char*> materials;
	std::vector<rg_mesh> meshes;
	cJSON* config;
	char levelname[64];
	rg_Resource* level_res;
	rg_jsState js_state;
} Level;

Level* rg_newLevel();
void rg_freeLevel(Level* level);
int rg_loadLevel(Level* level, rg_string level_name);
GameObject* rg_newGameobject();
void rg_freeGameobject(GameObject* ptr);
void rg_updateLevel(Level* level, double dt);

#endif /* RG_LEVEL_H_ */
