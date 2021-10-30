/*
 * vecmath.cpp
 *
 *  Created on: 3 апр. 2021 г.
 *      Author: alex9932
 */

#include "rg_vecmath.h"
#include "rg_engine.h"
#include <SDL2/SDL.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

//////////

static float math_inv_sqrt(float x) {
	return 1.0f / (float)SDL_sqrt(x);
}

static double math_inv_dsqrt(double x) {
	return 1.0 / SDL_sqrt(x);
}

//////////

void math_getForward(vec3* vec, vec2* angles) {
	vec->x = (float) SDL_sin(angles->x);
	vec->y = (float)-SDL_tan(angles->y);
	vec->z = (float)-SDL_cos(angles->x);
	vec3_normalize(vec, vec);
}

// vec2
void vec2_clone(vec2* dest, vec2* src) {
	dest->x = src->x;
	dest->y = src->y;
}

void vec2_add(vec2* dest, vec2* a, vec2* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}

void vec2_add(vec2* dest, vec2* a, float b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
}

void vec2_mul(vec2* dest, vec2* a, vec2* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
}

void vec2_mul(vec2* dest, vec2* a, float b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
}

void vec2_normalize(vec2* dest, vec2* a) {
	float inv_length = math_inv_sqrt(a->x * a->x + a->y * a->y);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
}

float vec2_length(vec2* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y);
}

float vec2_dot(vec2* a, vec2* b) {
	float q  = a->x * b->x;
		  q += a->y * b->y;
	return q;
}

void vec2_negate(vec2* dest, vec2* a) {
	dest->x = -a->x;
	dest->y = -a->y;
}

void vec2_lerp(vec2* dest, vec2* a, vec2* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
}

// vec3
void vec3_clone(vec3* dest, vec3* src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}

void vec3_add(vec3* dest, vec3* a, vec3* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

void vec3_add(vec3* dest, vec3* a, float b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
	dest->z = a->z + b;
}

void vec3_mul(vec3* dest, vec3* a, vec3* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
	dest->z = a->z * b->z;
}

void vec3_mul(vec3* dest, vec3* a, float b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
	dest->z = a->z * b;
}

void vec3_normalize(vec3* dest, vec3* a) {
	float inv_length = math_inv_sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
	dest->z = a->z * inv_length;
}

void vec3_cross(vec3* dest, vec3* a, vec3* b) {
	dest->x = a->y * b->z - a->z * b->y;
	dest->y = a->x * b->z - a->z * b->x;
	dest->z = a->x * b->y - a->y * b->x;
}

float vec3_length(vec3* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

float vec3_dot(vec3* a, vec3* b) {
	float q  = a->x * b->x;
		  q += a->y * b->y;
		  q += a->z * b->z;
	return q;
}

void vec3_negate(vec3* dest, vec3* a) {
	dest->x = -a->x;
	dest->y = -a->y;
	dest->z = -a->z;
}

void vec3_lerp(vec3* dest, vec3* a, vec3* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
	dest->z = a->z * (1.0 - delta) + b->z * delta;
}

// vec4
void vec4_clone(vec4* dest, vec4* src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
	dest->w = src->w;
}

void vec4_add(vec4* dest, vec4* a, vec4* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
	dest->w = a->w + b->w;
}

void vec4_add(vec4* dest, vec4* a, float b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
	dest->z = a->z + b;
	dest->w = a->w + b;
}

void vec4_mul(vec4* dest, vec4* a, vec4* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
	dest->z = a->z * b->z;
	dest->w = a->w * b->w;
}

void vec4_mul(vec4* dest, vec4* a, float b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
	dest->z = a->z * b;
	dest->w = a->w * b;
}

void vec4_normalize(vec4* dest, vec4* a) {
	float inv_length = math_inv_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
	dest->z = a->z * inv_length;
	dest->z = a->w * inv_length;
}

float vec4_length(vec4* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

float vec4_dot(vec4* a, vec4* b) {
	float q  = a->x * b->x;
		  q += a->y * b->y;
		  q += a->z * b->z;
		  q += a->w * b->w;
	return q;
}

void vec4_negate(vec4* dest, vec4* a) {
	dest->x = -a->x;
	dest->y = -a->y;
	dest->z = -a->z;
	dest->w = -a->w;
}

void vec4_lerp(vec4* dest, vec4* a, vec4* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
	dest->z = a->z * (1.0 - delta) + b->z * delta;
	dest->w = a->w * (1.0 - delta) + b->w * delta;
}



void quat_slerp(quat* dest, quat* q1, quat* q2, double lambda) {

//	double omega = acos(HeliMath::saturate(q0.mData[0]*q1.mData[0] +
//	                                           q0.mData[1]*q1.mData[1] +
//	                                           q0.mData[2]*q1.mData[2] +
//	                                           q0.mData[3]*q1.mData[3], -1,1));
//	    if (fabs(omega) < 1e-10) {
//	      omega = 1e-10;
//	    }
//	    double som = sin(omega);
//	    double st0 = sin((1-t) * omega) / som;
//	    double st1 = sin(t * omega) / som;
//
//	    return Quaternion(q0.mData[0]*st0 + q1.mData[0]*st1,
//	                      q0.mData[1]*st0 + q1.mData[1]*st1,
//	                      q0.mData[2]*st0 + q1.mData[2]*st1,
//	                      q0.mData[3]*st0 + q1.mData[3]*st1);

	float dotproduct = q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;
	float theta, st, sut, sout, coeff1, coeff2;

	// algorithm adapted from Shoemake's paper
//	lambda=lambda/2.0;

	theta = (float) acos(dotproduct);
	if (theta<0.0) theta=-theta;

	st = (float) sin(theta);
	sut = (float) sin(lambda*theta);
	sout = (float) sin((1-lambda)*theta);
	coeff1 = sout/st;
	coeff2 = sut/st;

	dest->x = coeff1*q1->x + coeff2*q2->x;
	dest->y = coeff1*q1->y + coeff2*q2->y;
	dest->z = coeff1*q1->z + coeff2*q2->z;
	dest->w = coeff1*q1->w + coeff2*q2->w;


	glm::quat x, y;
	x.x = q1->x;
	x.y = q1->y;
	x.z = q1->z;
	x.w = q1->w;
	y.x = q2->x;
	y.y = q2->y;
	y.z = q2->z;
	y.w = q2->w;
	glm::quat q = glm::slerp(x, y, (float)lambda);
	dest->x = q.x;
	dest->y = q.y;
	dest->z = q.z;
	dest->w = q.w;
}


// mat3

void mat3_identity(mat3* mat) {
	mat3_identity(mat, 1);
}

void mat3_identity(mat3* mat, float scale) {
	mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;
	mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;
	mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale;
}

void mat3_mul(vec3* dest, vec3* left, mat3* right) {
	dest->x = left->x * right->m00 + left->x * right->m01 + left->x * right->m02;
	dest->y = left->y * right->m10 + left->y * right->m11 + left->y * right->m12;
	dest->z = left->z * right->m20 + left->z * right->m21 + left->z * right->m22;
}

void mat3_mul(mat3* dest, mat3* left, mat3* right) {
	float* a = (float*)dest;
	float* b = (float*)left;
	float* c = (float*)right;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			float sum = 0.0;
			for (int k = 0; k < 3; k++) {
				sum = sum + b[i * 3 + k] * c[k * 3 + j];
			}
			a[i * 3 + j] = sum;
		}
	}
}

void mat3_rotx(mat3* mat, float angle) {
	mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;
	mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle);
	mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle);
}

void mat3_roty(mat3* mat, float angle) {
	mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle);
	mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;
	mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle);
}

void mat3_rotz(mat3* mat, float angle) {
	mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0;
	mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0;
	mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1;
}

void mat3_rotate(mat3* mat, float anglex, float angley, float anglez) {
	mat3 rx, ry, rz, rxy;
	mat3_rotx(&rx, anglex);
	mat3_roty(&ry, angley);
	mat3_rotz(&rz, anglez);
	mat3_mul(&rxy, &rx, &ry);
	mat3_mul(mat, &rxy, &rz);
}


// mat4
void mat4_identity(mat4* mat, vec3* scale) {
	mat->m00 = scale->x; mat->m01 = 0;        mat->m02 = 0;        mat->m03 = 0;
	mat->m10 = 0;        mat->m11 = scale->y; mat->m12 = 0;        mat->m13 = 0;
	mat->m20 = 0;        mat->m21 = 0;        mat->m22 = scale->z; mat->m23 = 0;
	mat->m30 = 0;        mat->m31 = 0;        mat->m32 = 0;        mat->m33 = 1;
}

void mat4_identity(mat4* mat, float scale) {
	mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;     mat->m03 = 0;
	mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;     mat->m13 = 0;
	mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale; mat->m23 = 0;
	mat->m30 = 0;     mat->m31 = 0;     mat->m32 = 0;     mat->m33 = 1;
}

void mat4_identity(mat4* mat) {
	mat4_identity(mat, 1);
}

void mat4_mul(vec4* dest, vec4* left, mat4* right) {
	dest->x = left->x * right->m00 + left->x * right->m01 + left->x * right->m02 + left->x * right->m03;
	dest->y = left->y * right->m10 + left->y * right->m11 + left->y * right->m12 + left->y * right->m13;
	dest->z = left->z * right->m20 + left->z * right->m21 + left->z * right->m22 + left->z * right->m23;
	dest->w = left->w * right->m30 + left->w * right->m31 + left->w * right->m32 + left->w * right->m33;
}

void mat4_mul(mat4* dest, mat4* left, mat4* right) {
	float* a = (float*)dest;
	float* b = (float*)left;
	float* c = (float*)right;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0.0;
			for (int k = 0; k < 4; k++) {
				sum = sum + b[i * 4 + k] * c[k * 4 + j];
			}
			a[i * 4 + j] = sum;
		}
	}


//	mat->m00 = m[0][0];
//	mat->m10 = m[1][0];
//	mat->m20 = m[2][0];
//	mat->m30 = m[3][0];
//	mat->m01 = m[0][1];
//	mat->m11 = m[1][1];
//	mat->m21 = m[2][1];
//	mat->m31 = m[3][1];
//	mat->m02 = m[0][2];
//	mat->m12 = m[1][2];
//	mat->m22 = m[2][2];
//	mat->m32 = m[3][2];
//	mat->m03 = m[0][3];
//	mat->m13 = m[1][3];
//	mat->m23 = m[2][3];
//	mat->m33 = m[3][3];

}

void mat4_rotx(mat4* mat, float angle) {
	mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;               mat->m03 = 0;
	mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle); mat->m13 = 0;
	mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle); mat->m23 = 0;
	mat->m30 = 0; mat->m31 = 0;              mat->m32 = 0;               mat->m33 = 1;
}

void mat4_roty(mat4* mat, float angle) {
	mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle); mat->m03 = 0;
	mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;              mat->m13 = 0;
	mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle); mat->m23 = 0;
	mat->m30 = 0;               mat->m31 = 0; mat->m32 = 0;              mat->m33 = 1;
}

void mat4_rotz(mat4* mat, float angle) {
	mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0; mat->m03 = 0;
	mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0; mat->m13 = 0;
	mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1; mat->m23 = 0;
	mat->m30 = 0;              mat->m31 = 0;               mat->m32 = 0; mat->m33 = 1;
}

void mat4_rotate(mat4* mat, float anglex, float angley, float anglez) {
	mat4 rx, ry, rz, ryz;
	mat4_rotx(&rx, anglex);
	mat4_roty(&ry, angley);
	mat4_rotz(&rz, anglez);
	mat4_mul(&ryz, &ry, &rz);
	mat4_mul(mat, &ryz, &rx);
//	mat4_mul(&rxy, &rx, &ry);
//	mat4_mul(mat, &rxy, &rz);
}

void mat4_quat(mat4* mat, quat* quat) {
	mat->m00 = 1.0f - 2.0f*quat->y*quat->y + 2.0f*quat->z*quat->z;
	mat->m10 =        2.0f*quat->x*quat->y + 2.0f*quat->w*quat->z;
	mat->m20 =        2.0f*quat->x*quat->z - 2.0f*quat->w*quat->y;

	mat->m01 =        2.0f*quat->x*quat->y - 2.0f*quat->w*quat->z;
	mat->m11 = 1.0f - 2.0f*quat->x*quat->x + 2.0f*quat->z*quat->z;
	mat->m21 =        2.0f*quat->y*quat->z + 2.0f*quat->w*quat->x;

	mat->m02 =        2.0f*quat->x*quat->z + 2.0f*quat->w*quat->y;
	mat->m12 =        2.0f*quat->y*quat->z - 2.0f*quat->w*quat->x;
	mat->m22 = 1.0f - 2.0f*quat->x*quat->x + 2.0f*quat->y*quat->y;

	glm::quat q;
	q.w = quat->w;
	q.x = quat->x;
	q.y = quat->y;
	q.z = quat->z;
	glm::mat4 m = glm::toMat4(q);
	float* glm_m = &m[0][0];
	mat->m00 = glm_m[0];
	mat->m10 = glm_m[1];
	mat->m20 = glm_m[2];
	mat->m30 = glm_m[3];
	mat->m01 = glm_m[4];
	mat->m11 = glm_m[5];
	mat->m21 = glm_m[6];
	mat->m31 = glm_m[7];
	mat->m02 = glm_m[8];
	mat->m12 = glm_m[9];
	mat->m22 = glm_m[10];
	mat->m32 = glm_m[11];
	mat->m03 = glm_m[12];
	mat->m13 = glm_m[13];
	mat->m23 = glm_m[14];
	mat->m33 = glm_m[15];
}

void mat4_translate(mat4* mat, float x, float y, float z) {
	mat->m03 = x;
	mat->m13 = y;
	mat->m23 = z;
}

void mat4_ortho(mat4* mat, float r, float l, float t, float b, float f, float n) {
	mat->m00 = 2 / (r - l); mat->m01 = 0;           mat->m02 = 0;           mat->m03 = -((r + l) / (r - l));
	mat->m10 = 0;           mat->m11 = 2 / (t - b); mat->m12 = 0;           mat->m13 = -((t + b) / (t - b));
	mat->m20 = 0;           mat->m21 = 0;           mat->m22 = 2 / (f - n); mat->m23 = -((f + n) / (f - n));
	mat->m30 = 0;           mat->m31 = 0;           mat->m32 = 0;           mat->m33 = 1;
}

void mat4_frustum(mat4* mat, float aspect, float fov, float near, float far) {
	float y_scale = (float)((1 / SDL_tan(math_toradians(fov / 2))));
	float x_scale = y_scale / aspect;
	float frustum_length = far - near;
	mat->m00 = x_scale; mat->m01 = 0;       mat->m02 = 0;                                mat->m03 = 0;
	mat->m10 = 0;       mat->m11 = y_scale; mat->m12 = 0;                                mat->m13 = 0;
	mat->m20 = 0;       mat->m21 = 0;       mat->m22 = -((far + near) / frustum_length); mat->m23 = -((2 * near * far) / frustum_length);
	mat->m30 = 0;       mat->m31 = 0;       mat->m32 = -1;                               mat->m33 = 0;
}

void mat4_view(mat4* mat, float x, float y, float z, float anglex, float angley) {
	mat4 rx;
	mat4 ry;
	mat4 translation;
	mat4 rotation;
	mat4_identity(&translation);
	mat4_roty(&rx, anglex);
	mat4_rotx(&ry, angley);
	mat4_translate(&translation, -x, -y, -z);
	mat4_mul(&rotation, &rx, &ry);
	mat4_mul(mat, &translation, &rotation);
}

void mat4_viewZ(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez) {
	mat4 translation;
	mat4 rotation;
	mat4_identity(&translation);
	mat4_rotate(&rotation, angley, anglex, anglez);
	mat4_translate(&translation, -x, -y, -z);
	mat4_mul(mat, &translation, &rotation);
}

void mat4_view_ruf(mat4* mat, vec3* right, vec3* up, vec3* forward, vec3* xyz) {
	mat4_view_ruf(mat, right->x, right->y, right->z, up->x, up->y, up->z, forward->x, forward->y, forward->z, xyz->x, xyz->y, xyz->z);
}

void mat4_view_ruf(mat4* mat, float rx, float ry, float rz, float ux, float uy, float uz, float fx, float fy, float fz, float x, float y, float z) {
	mat4 rmat;
	mat4 tmat;
	mat4_identity(&tmat);
	rmat.m00 = rx; rmat.m01 = ry; rmat.m02 = rz; rmat.m03 = 0;
	rmat.m10 = ux; rmat.m11 = uy; rmat.m12 = uz; rmat.m13 = 0;
	rmat.m20 = fx; rmat.m21 = fy; rmat.m22 = fz; rmat.m23 = 0;
	rmat.m30 = 0;  rmat.m31 = 0;  rmat.m32 = 0;  rmat.m33 = 1;
	mat4_translate(&tmat, -x, -y, -z);
	mat4_mul(mat, &tmat, &rmat);
}

void mat4_model(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez, float scale) {
	mat4 rotation;
	mat4 translation;
	mat4_identity(&translation, scale);
	mat4_rotate(&rotation, anglex, angley, anglez);
	mat4_translate(&translation, x, y, z);
	mat4_mul(mat, &rotation, &translation);
}

void mat4_model(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez) {
	mat4_model(mat, x, y, z, anglex, angley, anglez, 1);
}

void mat4_euler_rotation(mat4* mat, float roll, float pitch, float yaw) {
	mat4_identity(mat);

	double ch = SDL_cos(pitch);
	double sh = SDL_sin(pitch);
	double ca = SDL_cos(yaw);
	double sa = SDL_sin(yaw);
	double cb = SDL_cos(roll);
	double sb = SDL_sin(roll);

	mat->m00 = ch * ca;
	mat->m01 = sh * sb - ch * sa * cb;
	mat->m02 = ch * sa * sb + sh * cb;
	mat->m10 = sa;
	mat->m11 = ca * cb;
	mat->m12 = -ca * sb;
	mat->m20 = -sh * ca;
	mat->m21 = sh * sa * cb + ch * sb;
	mat->m22 = -sh * sa * sb + ch * cb;
}

void mat4_lookat(mat4* mat, vec3 pos, vec3 target, vec3 up) {
	vec3 fwd, ltmp;
	vec3 f, l, u;
	fwd.x = pos.x - target.x;
	fwd.y = pos.y - target.y;
	fwd.z = pos.z - target.z;
	vec3_normalize(&f, &fwd);
	vec3_cross(&ltmp, &up, &f);
	vec3_normalize(&l, &ltmp);
	vec3_cross(&u, &f, &l);
	mat4 rotation;
	rotation.m00 = l.x; rotation.m01 = l.y; rotation.m02 = l.z; rotation.m03 = 0;
	rotation.m10 = u.x; rotation.m11 = u.y; rotation.m12 = u.z; rotation.m13 = 0;
	rotation.m20 = f.x; rotation.m21 = f.y; rotation.m22 = f.z; rotation.m23 = 0;
	rotation.m30 = 0;   rotation.m31 = 0;   rotation.m32 = 0;   rotation.m33 = 1;
	mat4 translation;
	mat4_translate(&translation, pos.x, pos.y, pos.z);
	mat4_mul(mat, &rotation, &translation);
}

void mat4_invert(mat4* dest, mat4* src) {
//	float* m = (float*)src;
//	float* _dest = (float*)dest;
//	float inv[16];
//	inv[0]  =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
//	inv[4]  = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
//	inv[8]  =  m[4] * m[9]  * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
//	inv[12] = -m[4] * m[9]  * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
//	inv[1]  = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
//	inv[5]  =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
//	inv[9]  = -m[0] * m[9]  * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
//	inv[13] =  m[0] * m[9]  * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
//	inv[2]  =  m[1] * m[6]  * m[15] - m[1] * m[7]  * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7]  - m[13] * m[3] * m[6];
//	inv[6]  = -m[0] * m[6]  * m[15] + m[0] * m[7]  * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7]  + m[12] * m[3] * m[6];
//	inv[10] =  m[0] * m[5]  * m[15] - m[0] * m[7]  * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7]  - m[12] * m[3] * m[5];
//	inv[14] = -m[0] * m[5]  * m[14] + m[0] * m[6]  * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6]  + m[12] * m[2] * m[5];
//	inv[3]  = -m[1] * m[6]  * m[11] + m[1] * m[7]  * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9]  * m[2] * m[7]  + m[9]  * m[3] * m[6];
//	inv[7]  =  m[0] * m[6]  * m[11] - m[0] * m[7]  * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8]  * m[2] * m[7]  - m[8]  * m[3] * m[6];
//	inv[11] = -m[0] * m[5]  * m[11] + m[0] * m[7]  * m[9]  + m[4] * m[1] * m[11] - m[4] * m[3] * m[9]  - m[8]  * m[1] * m[7]  + m[8]  * m[3] * m[5];
//	inv[15] =  m[0] * m[5]  * m[10] - m[0] * m[6]  * m[9]  - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]  + m[8]  * m[1] * m[6]  - m[8]  * m[2] * m[5];
//
//	float det = 1.0 / (m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12]);
//
//	for (size_t i = 0; i < 16; i++) {
//		_dest[i] = inv[i] * det;
//	}

	glm::mat4 mat;
	float* glmm = &mat[0][0];
	glmm[0] = src->m00;
	glmm[1] = src->m10;
	glmm[2] = src->m20;
	glmm[3] = src->m30;
	glmm[4] = src->m01;
	glmm[5] = src->m11;
	glmm[6] = src->m21;
	glmm[7] = src->m31;
	glmm[8] = src->m02;
	glmm[9] = src->m12;
	glmm[10] = src->m22;
	glmm[11] = src->m32;
	glmm[12] = src->m03;
	glmm[13] = src->m13;
	glmm[14] = src->m23;
	glmm[15] = src->m33;
	glm::mat4 inv = glm::inverse(mat);
	float* glm_m = &inv[0][0];
	dest->m00 = glm_m[0];
	dest->m10 = glm_m[1];
	dest->m20 = glm_m[2];
	dest->m30 = glm_m[3];
	dest->m01 = glm_m[4];
	dest->m11 = glm_m[5];
	dest->m21 = glm_m[6];
	dest->m31 = glm_m[7];
	dest->m02 = glm_m[8];
	dest->m12 = glm_m[9];
	dest->m22 = glm_m[10];
	dest->m32 = glm_m[11];
	dest->m03 = glm_m[12];
	dest->m13 = glm_m[13];
	dest->m23 = glm_m[14];
	dest->m33 = glm_m[15];

}

// dvec2
void dvec2_clone(dvec2* dest, dvec2* src) {
	dest->x = src->x;
	dest->y = src->y;
}

void dvec2_add(dvec2* dest, dvec2* a, dvec2* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}

void dvec2_add(dvec2* dest, dvec2* a, double b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
}

void dvec2_mul(dvec2* dest, dvec2* a, dvec2* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
}

void dvec2_mul(dvec2* dest, dvec2* a, double b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
}

void dvec2_normalize(dvec2* dest, dvec2* a) {
	double inv_length = math_inv_dsqrt(a->x * a->x + a->y * a->y);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
}

double dvec2_length(dvec2* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y);
}

double dvec2_dot(dvec2* a, dvec2* b) {
	double q  = a->x * b->x;
		  q += a->y * b->y;
	return q;
}

void dvec2_negate(dvec2* dest, dvec2* a) {
	dest->x = -a->x;
	dest->y = -a->y;
}

void dvec2_lerp(dvec2* dest, dvec2* a, dvec2* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
}

// dvec3
void dvec3_clone(dvec3* dest, dvec3* src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}

void dvec3_add(dvec3* dest, dvec3* a, dvec3* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

void dvec3_add(dvec3* dest, dvec3* a, double b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
	dest->z = a->z + b;
}

void dvec3_mul(dvec3* dest, dvec3* a, dvec3* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
	dest->z = a->z * b->z;
}

void dvec3_mul(dvec3* dest, dvec3* a, double b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
	dest->z = a->z * b;
}

void dvec3_normalize(dvec3* dest, dvec3* a) {
	double inv_length = math_inv_dsqrt(a->x * a->x + a->y * a->y + a->z * a->z);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
	dest->z = a->z * inv_length;
}

void dvec3_cross(dvec3* dest, dvec3* a, dvec3* b) {
	dest->x = a->y * b->z - a->z * b->y;
	dest->y = a->x * b->z - a->z * b->x;
	dest->z = a->x * b->y - a->y * b->x;
}

double dvec3_length(dvec3* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

double dvec3_dot(dvec3* a, dvec3* b) {
	double q  = a->x * b->x;
		  q += a->y * b->y;
		  q += a->z * b->z;
	return q;
}

void dvec3_negate(dvec3* dest, dvec3* a) {
	dest->x = -a->x;
	dest->y = -a->y;
	dest->z = -a->z;
}

void dvec3_lerp(dvec3* dest, dvec3* a, dvec3* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
	dest->z = a->z * (1.0 - delta) + b->z * delta;
}

// dvec4
void dvec4_clone(dvec4* dest, dvec4* src) {
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
	dest->w = src->w;
}

void dvec4_add(dvec4* dest, dvec4* a, dvec4* b) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
	dest->w = a->w + b->w;
}

void dvec4_add(dvec4* dest, dvec4* a, double b) {
	dest->x = a->x + b;
	dest->y = a->y + b;
	dest->z = a->z + b;
	dest->w = a->w + b;
}

void dvec4_mul(dvec4* dest, dvec4* a, dvec4* b) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
	dest->z = a->z * b->z;
	dest->w = a->w * b->w;
}

void dvec4_mul(dvec4* dest, dvec4* a, double b) {
	dest->x = a->x * b;
	dest->y = a->y * b;
	dest->z = a->z * b;
	dest->w = a->w * b;
}

void dvec4_normalize(dvec4* dest, dvec4* a) {
	double inv_length = math_inv_dsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dest->x = a->x * inv_length;
	dest->y = a->y * inv_length;
	dest->z = a->z * inv_length;
	dest->z = a->w * inv_length;
}

double dvec4_length(dvec4* a) {
	return SDL_sqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

double dvec4_dot(dvec4* a, dvec4* b) {
	double q  = a->x * b->x;
		  q += a->y * b->y;
		  q += a->z * b->z;
		  q += a->w * b->w;
	return q;
}

void dvec4_negate(dvec4* dest, dvec4* a) {
	dest->x = -a->x;
	dest->y = -a->y;
	dest->z = -a->z;
	dest->w = -a->w;
}

void dvec4_lerp(dvec4* dest, dvec4* a, dvec4* b, double delta) {
	dest->x = a->x * (1.0 - delta) + b->x * delta;
	dest->y = a->y * (1.0 - delta) + b->y * delta;
	dest->z = a->z * (1.0 - delta) + b->z * delta;
	dest->w = a->w * (1.0 - delta) + b->w * delta;
}


// dmat3
void dmat3_identity(dmat3* mat) {
	dmat3_identity(mat, 1);
}

void dmat3_identity(dmat3* mat, double scale) {
	mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;
	mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;
	mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale;
}

void dmat3_mul(dvec3* dest, dvec3* left, dmat3* right) {
	dest->x = left->x * right->m00 + left->x * right->m01 + left->x * right->m02;
	dest->y = left->y * right->m10 + left->y * right->m11 + left->y * right->m12;
	dest->z = left->z * right->m20 + left->z * right->m21 + left->z * right->m22;
}

void dmat3_mul(dmat3* dest, dmat3* left, dmat3* right) {
	double* a = (double*)dest;
	double* b = (double*)left;
	double* c = (double*)right;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			double sum = 0.0;
			for (int k = 0; k < 3; k++) {
				sum = sum + b[i * 3 + k] * c[k * 3 + j];
			}
			a[i * 3 + j] = sum;
		}
	}
}

void dmat3_rotx(dmat3* mat, double angle) {
	mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;
	mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle);
	mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle);
}

void dmat3_roty(dmat3* mat, double angle) {
	mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle);
	mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;
	mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle);
}

void dmat3_rotz(dmat3* mat, double angle) {
	mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0;
	mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0;
	mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1;
}

void dmat3_rotate(dmat3* mat, double anglex, double angley, double anglez) {
	dmat3 rx, ry, rz, rxy;
	dmat3_rotx(&rx, anglex);
	dmat3_roty(&ry, angley);
	dmat3_rotz(&rz, anglez);
	dmat3_mul(&rxy, &rx, &ry);
	dmat3_mul(mat, &rxy, &rz);
}


// dmat4

void dmat4_identity(dmat4* mat, dvec3* scale) {
	mat->m00 = scale->x; mat->m01 = 0;        mat->m02 = 0;        mat->m03 = 0;
	mat->m10 = 0;        mat->m11 = scale->y; mat->m12 = 0;        mat->m13 = 0;
	mat->m20 = 0;        mat->m21 = 0;        mat->m22 = scale->z; mat->m23 = 0;
	mat->m30 = 0;        mat->m31 = 0;        mat->m32 = 0;        mat->m33 = 1;
}

void dmat4_identity(dmat4* mat, double scale) {
	mat->m00 = scale; mat->m01 = 0;     mat->m02 = 0;     mat->m03 = 0;
	mat->m10 = 0;     mat->m11 = scale; mat->m12 = 0;     mat->m13 = 0;
	mat->m20 = 0;     mat->m21 = 0;     mat->m22 = scale; mat->m23 = 0;
	mat->m30 = 0;     mat->m31 = 0;     mat->m32 = 0;     mat->m33 = 1;
}

void dmat4_identity(dmat4* mat) {
	dmat4_identity(mat, 1);
}

void dmat4_mul(dvec4* dest, dvec4* left, dmat4* right) {
	dest->x = left->x * right->m00 + left->x * right->m01 + left->x * right->m02 + left->x * right->m03;
	dest->y = left->y * right->m10 + left->y * right->m11 + left->y * right->m12 + left->y * right->m13;
	dest->z = left->z * right->m20 + left->z * right->m21 + left->z * right->m22 + left->z * right->m23;
	dest->w = left->w * right->m30 + left->w * right->m31 + left->w * right->m32 + left->w * right->m33;
}

void dmat4_mul(dmat4* dest, dmat4* left, dmat4* right) {
	double* a = (double*)dest;
	double* b = (double*)left;
	double* c = (double*)right;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			double sum = 0.0;
			for (int k = 0; k < 4; k++) {
				sum = sum + b[i * 4 + k] * c[k * 4 + j];
			}
			a[i * 4 + j] = sum;
		}
	}
}

void dmat4_rotx(dmat4* mat, double angle) {
	mat->m00 = 1; mat->m01 = 0;              mat->m02 = 0;               mat->m03 = 0;
	mat->m10 = 0; mat->m11 = SDL_cos(angle); mat->m12 = -SDL_sin(angle); mat->m13 = 0;
	mat->m20 = 0; mat->m21 = SDL_sin(angle); mat->m22 =  SDL_cos(angle); mat->m23 = 0;
	mat->m30 = 0; mat->m31 = 0;              mat->m32 = 0;               mat->m33 = 1;
}

void dmat4_roty(dmat4* mat, double angle) {
	mat->m00 =  SDL_cos(angle); mat->m01 = 0; mat->m02 = SDL_sin(angle); mat->m03 = 0;
	mat->m10 = 0;               mat->m11 = 1; mat->m12 = 0;              mat->m13 = 0;
	mat->m20 = -SDL_sin(angle); mat->m21 = 0; mat->m22 = SDL_cos(angle); mat->m23 = 0;
	mat->m30 = 0;               mat->m31 = 0; mat->m32 = 0;              mat->m33 = 1;
}

void dmat4_rotz(dmat4* mat, double angle) {
	mat->m00 = SDL_cos(angle); mat->m01 = -SDL_sin(angle); mat->m02 = 0; mat->m03 = 0;
	mat->m10 = SDL_sin(angle); mat->m11 =  SDL_cos(angle); mat->m12 = 0; mat->m13 = 0;
	mat->m20 = 0;              mat->m21 = 0;               mat->m22 = 1; mat->m23 = 0;
	mat->m30 = 0;              mat->m31 = 0;               mat->m32 = 0; mat->m33 = 1;
}

void dmat4_rotate(dmat4* mat, double anglex, double angley, double anglez) {
	dmat4 rx, ry, rz, rxy;
	dmat4_rotx(&rx, anglex);
	dmat4_roty(&ry, angley);
	dmat4_rotz(&rz, anglez);
	dmat4_mul(&rxy, &rx, &ry);
	dmat4_mul(mat, &rxy, &rz);
}

void dmat4_translate(dmat4* mat, double x, double y, double z) {
	mat->m03 = x;
	mat->m13 = y;
	mat->m23 = z;
}

void dmat4_ortho(dmat4* mat, double r, double l, double t, double b, double f, double n) {
	mat->m00 = 2 / (r - l); mat->m01 = 0;           mat->m02 = 0;           mat->m03 = -((r + l) / (r - l));
	mat->m10 = 0;           mat->m11 = 2 / (t - b); mat->m12 = 0;           mat->m13 = -((t + b) / (t - b));
	mat->m20 = 0;           mat->m21 = 0;           mat->m22 = 2 / (f - n); mat->m23 = -((f + n) / (f - n));
	mat->m30 = 0;           mat->m31 = 0;           mat->m32 = 0;           mat->m33 = 1;
}

void dmat4_frustum(dmat4* mat, double aspect, double fov, double near, double far) {
	double y_scale = (1.0 / tan(math_toradians(fov / 2.0))) * aspect;
	double x_scale = y_scale / aspect;
	double frustum_length = far - near;
	mat->m00 = x_scale; mat->m01 = 0;       mat->m02 = 0;                                mat->m03 = 0;
	mat->m10 = 0;       mat->m11 = y_scale; mat->m12 = 0;                                mat->m13 = 0;
	mat->m20 = 0;       mat->m21 = 0;       mat->m22 = -((far + near) / frustum_length); mat->m23 = -((2 * near * far) / frustum_length);
	mat->m30 = 0;       mat->m31 = 0;       mat->m32 = -1;                               mat->m33 = 0;
}

void dmat4_view(dmat4* mat, double x, double y, double z, double anglex, double angley) {
	dmat4 rx;
	dmat4 ry;
	dmat4 translation;
	dmat4 rotation;
	dmat4_identity(&translation);
	dmat4_roty(&rx, anglex);
	dmat4_rotx(&ry, angley);
	dmat4_translate(&translation, -x, -y, -z);
	dmat4_mul(&rotation, &rx, &ry);
	dmat4_mul(mat, &translation, &rotation);
}

void dmat4_view_ruf(dmat4* mat, dvec3* right, dvec3* up, dvec3* forward, dvec3* xyz) {
	dmat4_view_ruf(mat, right->x, right->y, right->z, up->x, up->y, up->z, forward->x, forward->y, forward->z, xyz->x, xyz->y, xyz->z);
}

void dmat4_view_ruf(dmat4* mat, double rx, double ry, double rz, double ux, double uy, double uz, double fx, double fy, double fz, double x, double y, double z) {
	dmat4 rmat;
	dmat4 tmat;
	dmat4_identity(&tmat);
	rmat.m00 = rx; rmat.m01 = ry; rmat.m02 = rz; rmat.m03 = 0;
	rmat.m10 = ux; rmat.m11 = uy; rmat.m12 = uz; rmat.m13 = 0;
	rmat.m20 = fx; rmat.m21 = fy; rmat.m22 = fz; rmat.m23 = 0;
	rmat.m30 = 0;  rmat.m31 = 0;  rmat.m32 = 0;  rmat.m33 = 1;
	dmat4_translate(&tmat, -x, -y, -z);
	dmat4_mul(mat, &tmat, &rmat);
}

void dmat4_model(dmat4* mat, double x, double y, double z, double anglex, double angley, double anglez, double scale) {
	dmat4 rotation;
	dmat4 translation;
	dmat4_identity(&translation, scale);
	dmat4_rotate(&rotation, anglex, angley, anglez);
	dmat4_translate(&translation, x, y, z);
	dmat4_mul(mat, &rotation, &translation);
}

void dmat4_model(dmat4* mat, double x, double y, double z, double anglex, double angley, double anglez) {
	dmat4_model(mat, x, y, z, anglex, angley, anglez, 1);
}

////////////////

void math_toDvec2(dvec2* dest, vec2* src) {
	dest->x = src->x;
	dest->x = src->y;
}

void math_toDvec3(dvec3* dest, vec3* src) {
	dest->x = src->x;
	dest->x = src->y;
	dest->z = src->z;
}

void math_toDvec4(dvec4* dest, vec4* src) {
	dest->x = src->x;
	dest->x = src->y;
	dest->z = src->z;
	dest->w = src->w;
}

void math_toVec2(vec2* dest, dvec2* src) {
	dest->x = (float)src->x;
	dest->x = (float)src->y;
}

void math_toVec3(vec3* dest, dvec3* src) {
	dest->x = (float)src->x;
	dest->x = (float)src->y;
	dest->z = (float)src->z;
}

void math_toVec4(vec4* dest, dvec4* src) {
	dest->x = (float)src->x;
	dest->x = (float)src->y;
	dest->z = (float)src->z;
	dest->w = (float)src->w;
}


void math_toDmat3(dmat3* dest, mat3* src) {
	double* _dest = (double*)dest;
	double* _src = (double*)src;
	for(uint i = 0; i < 9; i++) {
		_dest[i] = _src[i];
	}
}

void math_toDmat4(dmat4* dest, mat4* src) {
	double* _dest = (double*)dest;
	double* _src = (double*)src;
	for(uint i = 0; i < 16; i++) {
		_dest[i] = _src[i];
	}
}

void math_toMat3(mat3* dest, dmat3* src) {
	double* _dest = (double*)dest;
	double* _src = (double*)src;
	for(uint i = 0; i < 9; i++) {
		_dest[i] = (float)_src[i];
	}
}

void math_toMat4(mat4* dest, dmat4* src) {
	double* _dest = (double*)dest;
	double* _src = (double*)src;
	for(uint i = 0; i < 16; i++) {
		_dest[i] = (float)_src[i];
	}
}
