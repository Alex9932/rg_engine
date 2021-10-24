/*
 * fml2.h
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 *
 *  ! THIS LIBRARY IS DEPRECATED !
 *  ! THIS LIBRARY IS DEPRECATED !
 *  ! THIS LIBRARY IS DEPRECATED !
 *
 */

#ifndef FML2_H_
#define FML2_H_

typedef struct Model {
	unsigned int vertex_count;
	unsigned int index_count;
	float* vertices;
	float* normals;
	float* tangents;
	float* tex_coords;
	unsigned int* indices;
} Model;

/* ! DEPRECATED ! */
Model* FML2_fml_unpack(void* data);
/* ! DEPRECATED ! */
void* FML2_fml_pack(Model* mdl, int* length);

#endif /* FML2_H_ */
