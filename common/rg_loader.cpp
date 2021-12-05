/*
 * rg_loader.cpp
 *
 *  Created on: Oct 29, 2021
 *      Author: alex9932
 */

#include "rg_loader.h"
#include "rg_filesystem.h"
#include <stdio.h>

rg_Resource* rg_loadResource(rg_string name) {
	rg_Resource* res = rg_fs_getResource(name);

	if(res != NULL) {
		return res;
	}

	FILE* file = fopen(name, "rb");
	if(!file) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Resource %s not found!", name);
		return NULL;
	}

	res = (rg_Resource*)rg_malloc(sizeof(rg_Resource));

	fseek(file, 0, SEEK_END);
	res->length = ftell(file);
	rewind(file);
	res->data = rg_malloc(res->length + 1);
	((char*)res->data)[res->length] = '\0';
	memset(res->data, ' ', res->length);
	uint readed = fread(res->data, 1, res->length, file);
	if(readed != res->length && !feof(file)) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Error while reading file! Length/Readed: %d, %d", res->length, readed);
	}
	fclose(file);
	return res;
}

void rg_freeResource(rg_Resource* res) {
	rg_free(res->data);
	rg_free(res);
}

Animation* rg_loadAnimation(rg_string name) {
	rg_Resource* res = rg_loadResource(name);
	Animation* anim = rg_convertAnimation(res->data);
	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Loaded animation %s: %lf s.", name, anim->duration);
	rg_freeResource(res);
	return anim;
}

Animation* rg_convertAnimation(void* data) {
	AnimHeader* header = (AnimHeader*)data;
	data += sizeof(AnimHeader);

	Animation* anim = (Animation*)rg_malloc(sizeof(Animation));

	anim->bone_count = header->bone_count;
	anim->frame_count = header->frame_count;
	anim->tps = header->tps;
	anim->duration = header->duration;

	anim->frames = (KeyFrame*)rg_malloc(sizeof(KeyFrame) * anim->frame_count);
	for (size_t i = 0; i < anim->frame_count; ++i) {
		anim->frames[i].bones = (BoneTransform*)rg_malloc(sizeof(BoneTransform) * anim->bone_count);
		anim->frames[i].time_stamp = *(double*)data;
		data += sizeof(double);
		for (size_t j = 0; j < anim->bone_count; ++j) {
			BoneTransform* bt = (BoneTransform*)data;
			data += sizeof(BoneTransform);
			anim->frames[i].bones[j].id = bt->id;
			anim->frames[i].bones[j].position = bt->position;
			anim->frames[i].bones[j].scale = bt->scale;
			anim->frames[i].bones[j].orientation = bt->orientation;
		}
	}

	return anim;
}

void rg_writeAnimation(rg_string file, Animation* data) {
	FILE* f = fopen(file, "wb");
	AnimHeader header;
	header.magic[0] = 'a'; header.magic[1] = 'n'; header.magic[2] = 'i'; header.magic[3] = 'm';
	header.bone_count = data->bone_count;
	header.frame_count = data->frame_count;
	header.tps = data->tps;
	header.duration = data->duration;
	fwrite(&header, sizeof(AnimHeader), 1, f);

	for (size_t i = 0; i < data->frame_count; ++i) {
		fwrite(&data->frames[i].time_stamp, sizeof(double), 1, f);
		for (size_t j = 0; j < data->bone_count; ++j) {
			fwrite(&data->frames[i].bones[j], sizeof(BoneTransform), 1, f);
		}
	}

	fclose(f);
}

void rg_freeAnimation(Animation* anim) {
	for (size_t i = 0; i < anim->frame_count; ++i) {
		rg_free(anim->frames[i].bones);
	}
	rg_free(anim->frames);
	rg_free(anim);
}

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
