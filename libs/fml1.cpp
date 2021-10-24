/*
 * fml1.cpp
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 */

#include "fml1.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct FML_Header {
	Uint32 size_vertices;
	Uint32 size_normals;
	Uint32 size_tangents;
	Uint32 size_texcoords;
	Uint32 size_indices;
	Uint32 size_total;
};

float _fml_tofloat(char byte0, char byte1, char byte2, char byte3) {
	float output;
	*((Uint8*)(&output) + 3) = byte0;
	*((Uint8*)(&output) + 2) = byte1;
	*((Uint8*)(&output) + 1) = byte2;
	*((Uint8*)(&output) + 0) = byte3;
	return output;
}

Uint32 _fml_toint32(char byte0, char byte1, char byte2, char byte3) {
	Uint32 output;
	*((Uint8*)(&output) + 3) = byte0;
	*((Uint8*)(&output) + 2) = byte1;
	*((Uint8*)(&output) + 1) = byte2;
	*((Uint8*)(&output) + 0) = byte3;
	return output;
}

Uint64 _fml_toint64(char byte0, char byte1, char byte2, char byte3, char byte4, char byte5, char byte6, char byte7) {
	Uint64 output;
	*((Uint8*)(&output) + 7) = byte0;
	*((Uint8*)(&output) + 6) = byte1;
	*((Uint8*)(&output) + 5) = byte2;
	*((Uint8*)(&output) + 4) = byte3;
	*((Uint8*)(&output) + 3) = byte4;
	*((Uint8*)(&output) + 2) = byte5;
	*((Uint8*)(&output) + 1) = byte6;
	*((Uint8*)(&output) + 0) = byte7;
	return output;
}

void _fml_dbg32(Uint32 n) {
	int c, k;

	printf("0x%x in binary: ", n);
	for (c = 31; c >= 0; c--) {
		k = n >> c;
		if (k & 1) {
			printf("1");
		}
		else {
			printf("0");
		}
	}

	printf("\n");
}

void _fml_dbg64(Uint64 n) {
	int c, k;

	printf("0x%x in binary: ", n);
	for (c = 63; c >= 0; c--) {
		k = n >> c;
		if (k & 1) {
			printf("1");
		}
		else {
			printf("0");
		}
	}

	printf("\n");
}

FMLModelData* FML1_o_fml_load(const char* path, Uint8 flag) {
	printf("[fml] Loading model: %s ...", path);
	FILE *file = fopen(path, "rb");
	if (!file) {
		printf("  ERROR!\n");
		printf("[fml] %s: invalid file!\n", path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	Uint64 length = ftell(file);
	rewind(file);
	char* data = (char*)malloc(length);
	fread(data, length, 1, file);
	fclose(file);

	FML_Header* header = (FML_Header*)malloc(sizeof(FML_Header));
	header->size_vertices = _fml_toint32(data[0], data[1], data[2], data[3]);
	header->size_normals = _fml_toint32(data[4], data[5], data[6], data[7]);
	header->size_tangents = _fml_toint32(data[8], data[9], data[10], data[11]);
	header->size_texcoords = _fml_toint32(data[12], data[13], data[14], data[15]);
	header->size_indices = _fml_toint32(data[16], data[17], data[18], data[19]);
	header->size_total = _fml_toint64(data[20], data[21], data[22], data[23], data[24], data[25], data[26], data[27]);

	//Next 28

	Uint64 total_size = (Uint64)header->size_vertices + (Uint64)header->size_normals + (Uint64)header->size_tangents + (Uint64)header->size_texcoords + (Uint64)header->size_indices;

	if (header->size_total != total_size) {
		printf("  ERROR!\n");
		printf("[fml] Bad file header! => %s\n", path);
		printf("[fml] Total in header: %d, in file: %d\n", header->size_total, total_size);

		printf("[fml] Total\n");
		_fml_dbg64(header->size_total);
		_fml_dbg64(total_size);

		printf("[fml] Header info:\n");
		printf("[fml] size_vertices: ");
		_fml_dbg32(header->size_vertices);
		printf("[fml] size_normals: ");
		_fml_dbg32(header->size_normals);
		printf("[fml] size_tangents: ");
		_fml_dbg32(header->size_tangents);
		printf("[fml] size_texcoords: ");
		_fml_dbg32(header->size_texcoords);
		printf("[fml] size_indices: ");
		_fml_dbg32(header->size_indices);
		return NULL;
	}

	// Read verteces data

	FMLModelData* model = (FMLModelData*)malloc(sizeof(FMLModelData));

	model->vertices_ptr = (float*)malloc(sizeof(float) * header->size_vertices);

	if(flag == FML_FULL_MODEL) {
		model->normals_ptr = (float*)malloc(sizeof(float) * header->size_normals);
		model->tangents_ptr = (float*)malloc(sizeof(float) * header->size_tangents);
		model->texcoords_ptr = (float*)malloc(sizeof(float) * header->size_texcoords);
	}

	model->indices_ptr = (Uint32*)malloc(sizeof(Uint32) * header->size_indices);
	model->vertices_size = header->size_vertices;

	if(flag == FML_FULL_MODEL) {
		model->normals_size = header->size_normals;
		model->tangents_size = header->size_tangents;
		model->texcoords_size = header->size_texcoords;
	}

	model->indices_size = header->size_indices;

	int num = 28;
	for (Uint32 i = 0; i < header->size_vertices; i++) {
		model->vertices_ptr[i] = _fml_tofloat(data[num], data[num + 1], data[num + 2], data[num + 3]);
		num += 4;
	}

	if(flag == FML_FULL_MODEL) {
		for (Uint32 i = 0; i < header->size_normals; i++) {
			model->normals_ptr[i] = _fml_tofloat(data[num], data[num + 1], data[num + 2], data[num + 3]);
			num += 4;
		}

		for (Uint32 i = 0; i < header->size_tangents; i++) {
			model->tangents_ptr[i] = _fml_tofloat(data[num], data[num + 1], data[num + 2], data[num + 3]);
			num += 4;
		}

		for (Uint32 i = 0; i < header->size_texcoords; i++) {
			model->texcoords_ptr[i] = _fml_tofloat(data[num], data[num + 1], data[num + 2], data[num + 3]);
			num += 4;
		}
	}
	for (Uint32 i = 0; i < header->size_indices; i++) {
		model->indices_ptr[i] = _fml_toint32(data[num], data[num + 1], data[num + 2], data[num + 3]);
		num += 4;
	}

	printf("  OK!\n");

	free(header);
	free(data);

	return model;
}

void FML1_model_free_modeldata(FMLModelData* data) {
	free(data->vertices_ptr);
	free(data->normals_ptr);
	free(data->tangents_ptr);
	free(data->texcoords_ptr);
	free(data->indices_ptr);
	free(data);
}

