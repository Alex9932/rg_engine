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

RG_INLINE static void __assembly_path(char* dest, size_t len, rg_string path, rg_string name) {
	SDL_memset(dest, '\0', len);
	strcpy(dest, path);
	strcat(dest, "/");
	strcat(dest, name);
}

void cl_mat_load(rg_string name) {
	char path[128];
	rg_buildResourcePath(name, path, RG_MATERIALS_DIR);

	char diff[128];
	char norm[128];
	char roug[128];
	char metl[128];

	__assembly_path(diff, 128, path, "diff.png");
	__assembly_path(norm, 128, path, "norm.png");
	__assembly_path(roug, 128, path, "roug.png");
	__assembly_path(metl, 128, path, "metl.png");

	rg_material_t* mat = (rg_material_t*)rg_malloc(sizeof(rg_material_t));
	mat->diffuse = cl_getTexture(diff, CL_TEXTURE_DIFFUSE);
	mat->normal = cl_getTexture(norm, CL_TEXTURE_NORMAL);
	mat->roughness = cl_getTexture(roug, CL_TEXTURE_ROUGHNESS);
	mat->metallic = cl_getTexture(metl, CL_TEXTURE_METALLIC);
	mat->mat_id = materials.size();
	materials.push_back(mat);
}

void _cl_mat_unload(rg_material_t* mat) {
	cl_deleteTexture(mat->diffuse);
	cl_deleteTexture(mat->normal);
	cl_deleteTexture(mat->roughness);
	cl_deleteTexture(mat->metallic);
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
