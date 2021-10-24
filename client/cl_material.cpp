/*
 * cl_material.cpp
 *
 *  Created on: 28 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_material.h"
#include <rg_engine.h>
#include <vector>
#include <cstring>

static std::vector<rg_material_t*> materials;

void cl_mat_load(rg_string name) {
	char path[128];
	rg_buildResourcePath(rg_level->levelname, name, path, RG_MATERIALS_DIR);

	char diff[128];
	char norm[128];
	SDL_memset(diff, '\0', 128);
	SDL_memset(norm, '\0', 128);
	strcpy(diff, path);
	strcpy(norm, path);
	strcat(diff, "/");
	strcat(norm, "/");
	strcat(diff, "diff.png");
	strcat(norm, "norm.png");
	rg_material_t* mat = (rg_material_t*)rg_malloc(sizeof(rg_material_t));
	mat->diffuse = cl_getTexture(diff);
	mat->normal = cl_getTexture(norm);
	mat->mat_id = materials.size();
	materials.push_back(mat);
}

void _cl_mat_unload(rg_material_t* mat) {
	cl_deleteTexture(mat->diffuse);
	cl_deleteTexture(mat->normal);
	rg_free(mat);
}

void cl_mat_unloadAll() {
	for (Uint32 i = 0; i < materials.size(); ++i) {
		_cl_mat_unload(materials[i]);
	}
	materials.clear();
}

rg_material_t* cl_mat_get(Uint32 mat_id) {
	return materials[mat_id];
}
