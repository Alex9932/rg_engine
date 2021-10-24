/*
 * rg_modelloader.cpp
 *
 *  Created on: 13 апр. 2021 г.
 *      Author: alex9932
 */

#include "rg_modelloader.h"
#include <stdio.h>

rg_mesh_t* rg_rmlConvert(void* data) {
	rg_rmlHeader_t header = *(rg_rmlHeader_t*)data;
	data += sizeof(rg_rmlHeader_t);
	rg_vertex_t* vertices = (rg_vertex_t*)data;
	data += sizeof(rg_vertex_t) * header.vertices;
	Uint32* indices = (Uint32*)data;

	rg_mesh_t* mesh = rg_newMesh();
	mesh->vertex_count = header.vertices;
	mesh->index_count = header.indices;
	mesh->vertices = vertices;
	mesh->indices = indices;
	return mesh;
}

void rg_rmlWrite(rg_string file, rg_mesh_t* mesh) {
	FILE* f = fopen(file, "wb");
	rg_rmlHeader_t header;
	header.magic[0] = 'r'; header.magic[1] = 'm'; header.magic[2] = 'l'; header.magic[3] = '1';
	header.vertices = mesh->vertex_count;
	header.indices = mesh->index_count;
	header.rml_version = 1;
	fwrite(&header, sizeof(rg_rmlHeader_t), 1, f);
	fwrite(mesh->vertices, sizeof(rg_vertex_t), header.vertices, f);
	fwrite(mesh->indices, sizeof(Uint32), header.indices, f);
	fclose(f);
}
