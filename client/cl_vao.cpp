/*
 * cl_vao.cpp
 *
 *  Created on: 12 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_vao.h"
#include <rg_engine.h>

cl_VAO* cl_makeVAO(rg_mesh_t* mesh) {
	return cl_makeVAO(mesh, GL_STATIC_DRAW);
}

cl_VAO* cl_makeVAO(rg_mesh_t* mesh, GLuint drawType) {
	cl_VAO* vao = (cl_VAO*)rg_malloc(sizeof(cl_VAO));
	vao->anim = false;
	vao->i_count = mesh->index_count;
	int vertex_size = sizeof(rg_vertex_t);

	// Generate vertex array object
	glGenVertexArrays(1, &vao->vao);
	glBindVertexArray(vao->vao);

	glGenBuffers(1, &vao->vbo);
	glGenBuffers(1, &vao->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * vertex_size, mesh->vertices, drawType);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 0)); // vXYZ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 3)); // nXYZ
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 6)); // tXYZ
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_size, (GLvoid*)(sizeof(float) * 9)); // UV
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Uint32) * mesh->index_count, mesh->indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return vao;
}

cl_VAO* cl_makeAVAO(AnimatedMesh* mesh) {
	cl_VAO* vao = cl_makeVAO(&mesh->mesh);
	vao->anim = true;
	glBindVertexArray(vao->vao);

	int weight_size = sizeof(Weights);
	glGenBuffers(1, &vao->wvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->wvbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->mesh.vertex_count * weight_size, mesh->weights, GL_STATIC_DRAW);
	glVertexAttribIPointer(4, 4, GL_INT, weight_size, (GLvoid*)(sizeof(Sint32) * 0)); // bone ids
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, weight_size, (GLvoid*)(sizeof(float) * 4)); // weights
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);
	return vao;
}

void cl_deleteVAO(cl_VAO* vao) {
	glDeleteVertexArrays(1, &vao->vao);
	glDeleteBuffers(1, &vao->vbo);
	glDeleteBuffers(1, &vao->ebo);
	rg_free(vao);
}

void cl_drawVAO(cl_VAO* vao) {
	glBindVertexArray(vao->vao);
	glDrawElements(GL_TRIANGLES, vao->i_count, GL_UNSIGNED_INT, 0);
}
