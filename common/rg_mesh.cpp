/*
 * rg_mesh.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#include "rg_mesh.h"
#include "rg_engine.h"

rg_mesh_t* rg_newMesh() {
	return (rg_mesh_t*)rg_malloc(sizeof(rg_mesh_t));
}

void rg_freeMesh(rg_mesh_t* mesh) {
	rg_free(mesh);
}
