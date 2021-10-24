/*
 * fml1.h
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 *
 *  ! THIS LIBRARY IS DEPRECATED !
 *  ! THIS LIBRARY IS DEPRECATED !
 *  ! THIS LIBRARY IS DEPRECATED !
 *
 */

#ifndef FML1_H_
#define FML1_H_

#define FML_FULL_MODEL 0x00
#define FML_VERTICES_ONLY 0x01

typedef struct FMLModelData FMLModelData;
typedef struct FML_Header FML_Header;

struct FMLModelData {
	float* vertices_ptr;
	float* normals_ptr;
	float* tangents_ptr;
	float* texcoords_ptr;
	unsigned int* indices_ptr;
	unsigned int vertices_size;
	unsigned int normals_size;
	unsigned int tangents_size;
	unsigned int texcoords_size;
	unsigned int indices_size;
};

/* ! DEPRECATED ! */
FMLModelData* FML1_o_fml_load(const char* path, unsigned char flag);
/* ! DEPRECATED ! */
void FML1_model_free_modeldata(FMLModelData* data);

#endif /* FML1_H_ */
