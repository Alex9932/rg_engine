/*
 * rg_mesh.h
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_MESH_H_
#define RG_MESH_H_

#include <SDL2/SDL.h>
#include "rg_vecmath.h"

typedef struct rg_vertex_t {
	vec3 vertex;
	vec3 normal;
	vec3 tangent;
	vec2 t_coord;
} rg_vertex_t;

typedef struct rg_mesh_t {
	rg_vertex_t* vertices;
	Uint32* indices;
	Uint32 vertex_count;
	Uint32 index_count;
} rg_mesh_t;

rg_mesh_t* rg_newMesh(); // !!! NOT ALLOCATE MEMORY FOR "vertices" AND "indices" FIELDS !!!
void rg_freeMesh(rg_mesh_t* mesh);

#endif /* RG_MESH_H_ */
