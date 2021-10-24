/*
 * fml2.cpp
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 */

#include "fml2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char magic[4];
	unsigned int vertex_count;
	unsigned int index_count;
} FML2_Header;

void* FML2_fml_pack(Model* mdl, int* length) {
	printf("[fml2] Packing data");
	FML2_Header header;
	header.magic[0] = 'F'; header.magic[1] = 'M'; header.magic[2] = 'L'; header.magic[3] = '2';
	header.vertex_count = mdl->vertex_count;
	header.index_count = mdl->index_count;

	void* data = malloc(sizeof(FML2_Header)   // Header
		+ sizeof(float)*3*header.vertex_count // Vertices
		+ sizeof(float)*3*header.vertex_count // Normals
		+ sizeof(float)*3*header.vertex_count // Tangents
		+ sizeof(float)*2*header.vertex_count // Texture coords
		+ sizeof(int)*header.index_count      // Indices
	);

	memcpy(data, &header, sizeof(FML2_Header));
	void* pointer = data + sizeof(FML2_Header);
	memcpy(pointer, mdl->vertices, sizeof(float)*3*header.vertex_count);
	pointer += sizeof(float) * header.vertex_count * 3;
	memcpy(pointer, mdl->normals, sizeof(float)*3*header.vertex_count);
	pointer += sizeof(float) * header.vertex_count * 3;
	memcpy(pointer, mdl->tangents, sizeof(float)*3*header.vertex_count);
	pointer += sizeof(float) * header.vertex_count * 3;
	memcpy(pointer, mdl->tex_coords, sizeof(float)*2*header.vertex_count);
	pointer += sizeof(float) * header.vertex_count * 2;
	memcpy(pointer, mdl->indices, sizeof(int)*header.index_count);
	pointer += sizeof(unsigned int) * header.index_count;
	*length = (long)pointer - (long)data;
	printf("    OK!\n");

	return data;
}

Model* FML2_fml_unpack(void* data) {
	FML2_Header header;
	memcpy(&header, data, sizeof(FML2_Header));
	/*printf("magic: %s\n", header.magic);
	printf("verts: %f\n", header.vertex_count);
	printf("inds: %f\n", header.index_count);*/

	Model* model = (Model*)malloc(sizeof(Model));
	model->vertex_count = header.vertex_count;
	model->index_count = header.index_count;
	model->vertices = (float*)malloc(sizeof(float) * model->vertex_count * 3);
	model->normals = (float*)malloc(sizeof(float) * model->vertex_count * 3);
	model->tangents = (float*)malloc(sizeof(float) * model->vertex_count * 3);
	model->tex_coords = (float*)malloc(sizeof(float) * model->vertex_count * 2);
	model->indices = (unsigned int*)malloc(sizeof(int) * model->index_count);

	void* pointer = data + sizeof(FML2_Header);
	memcpy(model->vertices, pointer, sizeof(float) * model->vertex_count * 3);
	pointer += sizeof(float) * model->vertex_count * 3;
	memcpy(model->normals, pointer, sizeof(float) * model->vertex_count * 3);
	pointer += sizeof(float) * model->vertex_count * 3;
	memcpy(model->tangents, pointer, sizeof(float) * model->vertex_count * 3);
	pointer += sizeof(float) * model->vertex_count * 3;
	memcpy(model->tex_coords, pointer, sizeof(float) * model->vertex_count * 2);
	pointer += sizeof(float) * model->vertex_count * 2;
	memcpy(model->indices, pointer, sizeof(int) * model->index_count);
	return model;
}

