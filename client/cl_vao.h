/*
 * cl_vao.h
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef CL_VAO_H_
#define CL_VAO_H_

#include <GL/glew.h>
#include <rg_anim.h>
#include <rg_mesh.h>

typedef struct cl_VAO {
	GLuint vao;
	GLuint vbo; // Vertices, normals, tangents and texture coordinates
	GLuint wvbo; // For animated model (weights, bone ids)
	GLuint ebo;
	Uint32 i_count;
	bool anim;
} cl_VAO;

cl_VAO* cl_makeVAO(rg_mesh_t* mesh);
cl_VAO* cl_makeVAO(rg_mesh_t* mesh, GLuint drawType);
cl_VAO* cl_makeAVAO(AnimatedMesh* mesh);
void cl_deleteVAO(cl_VAO* vao);
void cl_drawVAO(cl_VAO* vao);

#endif /* CL_VAO_H_ */
