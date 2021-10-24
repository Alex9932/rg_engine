/*
 * rg_animmodelloader.cpp
 *
 *  Created on: Oct 5, 2021
 *      Author: alex9932
 */

#include "rg_animmodelloader.h"
#include <stdio.h>

AnimatedMesh* rg_armlConvert(void* data) {
	rg_rmlHeader_t header = *(rg_rmlHeader_t*)data;
	data += sizeof(rg_rmlHeader_t);
	rg_vertex_t* vertices = (rg_vertex_t*)data;
	data += sizeof(rg_vertex_t) * header.vertices;
	Uint32* indices = (Uint32*)data;
	data += sizeof(Uint32) * header.indices;
	Weights* weights = (Weights*)data;
	data += sizeof(Weights) * header.vertices;
	Uint32* boneCount = (Uint32*)data;
	data += sizeof(Uint32);
	Bone* bones = (Bone*)data;

	// DEBUG
//	printf("Loaded mesh -> v: %d i: %d b: %d\n", header.vertices, header.indices, *boneCount);

	AnimatedMesh* mesh = (AnimatedMesh*)rg_malloc(sizeof(AnimatedMesh));
	mesh->mesh.vertex_count = header.vertices;
	mesh->mesh.index_count = header.indices;
	mesh->mesh.vertices = vertices;
	mesh->mesh.indices = indices;
	mesh->weights = weights;
	mesh->skeleton.bone_count = *boneCount;
	mesh->skeleton.bones = bones;
	return mesh;
}

void rg_armlWrite(rg_string file, AnimatedMesh* mesh) {
	FILE* f = fopen(file, "wb");
	rg_rmlHeader_t header;
	header.magic[0] = 'r'; header.magic[1] = 'm'; header.magic[2] = 'l'; header.magic[3] = '1';
	header.vertices = mesh->mesh.vertex_count;
	header.indices = mesh->mesh.index_count;
	header.rml_version = 2;
	fwrite(&header, sizeof(rg_rmlHeader_t), 1, f);
	fwrite(mesh->mesh.vertices, sizeof(rg_vertex_t), header.vertices, f);
	fwrite(mesh->mesh.indices, sizeof(Uint32), header.indices, f);
	fwrite(mesh->weights, sizeof(Weights), header.vertices, f);
	fwrite(&mesh->skeleton.bone_count, sizeof(Uint32), 1, f);
	fwrite(mesh->skeleton.bones, sizeof(Bone), mesh->skeleton.bone_count, f);
	fclose(f);
}
