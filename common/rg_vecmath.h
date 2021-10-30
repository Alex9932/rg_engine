/*
 * vecmath.h
 *
 *  Created on: 3 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_VECMATH_H_
#define RG_VECMATH_H_

#include "rg_engine.h"

#ifndef PI
#define PI 3.1415926535
#endif

typedef struct vec2 { float x; float y; } vec2;
typedef struct vec3 { float x; float y; float z; } vec3;
typedef struct vec4 { float x; float y; float z; float w; } vec4;
typedef struct dvec2 { double x; float y; } dvec2;
typedef struct dvec3 { double x; float y; float z; } dvec3;
typedef struct dvec4 { double x; float y; float z; float w; } dvec4;

typedef vec4 quat;

//typedef struct quat { float w; float x; float y; float z; } quat;

typedef dvec4 dquat;

typedef struct mat3 { float m00; float m10; float m20;
					  float m01; float m11; float m21;
					  float m02; float m12; float m22; } mat3;

typedef struct mat4 { float m00; float m10; float m20; float m30;
					  float m01; float m11; float m21; float m31;
					  float m02; float m12; float m22; float m32;
					  float m03; float m13; float m23; float m33; } mat4;

typedef struct dmat3 { double m00; double m10; double m20;
					   double m01; double m11; double m21;
					   double m02; double m12; double m22; } dmat3;

typedef struct dmat4 { double m00; double m10; double m20; double m30;
					   double m01; double m11; double m21; double m31;
					   double m02; double m12; double m22; double m32;
					   double m03; double m13; double m23; double m33; } dmat4;

RG_INLINE double math_toradians(double angle) {
	return (angle * (PI / 180.0));
}

RG_INLINE double math_todegrees(double angle) {
	return (angle * (180.0 / PI));
}

RG_INLINE vec2* math_newVec2() { return (vec2*)rg_malloc(sizeof(vec2)); }
RG_INLINE vec3* math_newVec3() { return (vec3*)rg_malloc(sizeof(vec3)); }
RG_INLINE vec4* math_newVec4() { return (vec4*)rg_malloc(sizeof(vec4)); }
RG_INLINE mat3* math_newMat3() { return (mat3*)rg_malloc(sizeof(mat3)); }
RG_INLINE mat4* math_newMat4() { return (mat4*)rg_malloc(sizeof(mat4)); }
RG_INLINE dvec2* math_newDVec2() { return (dvec2*)rg_malloc(sizeof(dvec2)); }
RG_INLINE dvec3* math_newDVec3() { return (dvec3*)rg_malloc(sizeof(dvec3)); }
RG_INLINE dvec4* math_newDVec4() { return (dvec4*)rg_malloc(sizeof(dvec4)); }
RG_INLINE dmat3* math_newDMat3() { return (dmat3*)rg_malloc(sizeof(dmat3)); }
RG_INLINE dmat4* math_newDMat4() { return (dmat4*)rg_malloc(sizeof(dmat4)); }
RG_INLINE void math_delete(void* ptr) { rg_free(ptr); }

void math_getForward(vec3* vec, vec2* angles);

// vec2
void vec2_clone(vec2* dest, vec2* src);
void vec2_add(vec2* dest, vec2* a, vec2* b);
void vec2_add(vec2* dest, vec2* a, float b);
void vec2_mul(vec2* dest, vec2* a, vec2* b);
void vec2_mul(vec2* dest, vec2* a, float b);
void vec2_normalize(vec2* dest, vec2* a);
float vec2_length(vec2* a);
float vec2_dot(vec2* a, vec2* b);
void vec2_negate(vec2* dest, vec2* a);
void vec2_lerp(vec2* dest, vec2* a, vec2* b, double delta);

// vec3
void vec3_clone(vec3* dest, vec3* src);
void vec3_add(vec3* dest, vec3* a, vec3* b);
void vec3_add(vec3* dest, vec3* a, float b);
void vec3_mul(vec3* dest, vec3* a, vec3* b);
void vec3_mul(vec3* dest, vec3* a, float b);
void vec3_normalize(vec3* dest, vec3* a);
void vec3_cross(vec3* dest, vec3* a, vec3* b);
float vec3_length(vec3* a);
float vec3_dot(vec3* a, vec3* b);
void vec3_negate(vec3* dest, vec3* a);
void vec3_lerp(vec3* dest, vec3* a, vec3* b, double delta);

// vec4
void vec4_clone(vec4* dest, vec4* src);
void vec4_add(vec4* dest, vec4* a, vec4* b);
void vec4_add(vec4* dest, vec4* a, float b);
void vec4_mul(vec4* dest, vec4* a, vec4* b);
void vec4_mul(vec4* dest, vec4* a, float b);
void vec4_normalize(vec4* dest, vec4* a);
float vec4_length(vec4* a);
float vec4_dot(vec4* a, vec4* b);
void vec4_negate(vec4* dest, vec4* a);
void vec4_lerp(vec4* dest, vec4* a, vec4* b, double delta);

void quat_slerp(quat* dest, quat* q1, quat* q2, double lambda);

// mat3
void mat3_identity(mat3* mat);
void mat3_identity(mat3* mat, float scale);
void mat3_mul(vec3* dest, vec3* left, mat3* right);
void mat3_mul(mat3* dest, mat3* left, mat3* right);
void mat3_rotx(mat3* mat, float angle);
void mat3_roty(mat3* mat, float angle);
void mat3_rotz(mat3* mat, float angle);
void mat3_rotate(mat3* mat, float anglex, float angley, float anglez);

// mat4
void mat4_identity(mat4* mat);
void mat4_identity(mat4* mat, float scale);
void mat4_identity(mat4* mat, vec3* scale);
void mat4_mul(vec4* dest, vec4* left, mat4* right);
void mat4_mul(mat4* dest, mat4* left, mat4* right);

void mat4_rotx(mat4* mat, float angle);
void mat4_roty(mat4* mat, float angle);
void mat4_rotz(mat4* mat, float angle);
void mat4_rotate(mat4* mat, float anglex, float angley, float anglez);
void mat4_quat(mat4* mat, quat* quat);
void mat4_translate(mat4* mat, float x, float y, float z);

void mat4_ortho(mat4* mat, float r, float l, float t, float b, float f, float n );
void mat4_frustum(mat4* mat, float aspect, float fov, float near_plane, float far_plane);
void mat4_view(mat4* mat, float x, float y, float z, float anglex, float angley);
void mat4_viewZ(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez);
void mat4_view_ruf(mat4* mat, float rx, float ry, float rz, float ux, float uy, float uz, float fx, float fy, float fz, float x, float y, float z);
void mat4_view_ruf(mat4* mat, vec3* right, vec3* up, vec3* forward, vec3* xyz);
void mat4_model(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez, float scale);
void mat4_model(mat4* mat, float x, float y, float z, float anglex, float angley, float anglez);
void mat4_euler_rotation(mat4* mat, float roll, float pitch, float yaw);
void mat4_lookat(mat4* mat, vec3 pos, vec3 target, vec3 up);
void mat4_invert(mat4* dest, mat4* src);
//////////////////////////////////////////////

// vec2
void dvec2_clone(dvec2* dest, dvec2* src);
void dvec2_add(dvec2* dest, dvec2* a, dvec2* b);
void dvec2_add(dvec2* dest, dvec2* a, double b);
void dvec2_mul(dvec2* dest, dvec2* a, dvec2* b);
void dvec2_mul(dvec2* dest, dvec2* a, double b);
void dvec2_normalize(dvec2* dest, dvec2* a);
double dvec2_length(dvec2* a);
double dvec2_dot(dvec2* a, dvec2* b);
void dvec2_negate(dvec2* dest, dvec2* a);
void dvec2_lerp(dvec2* dest, dvec2* a, dvec2* b, double delta);

// vec3
void dvec3_clone(dvec3* dest, dvec3* src);
void dvec3_add(dvec3* dest, dvec3* a, dvec3* b);
void dvec3_add(dvec3* dest, dvec3* a, double b);
void dvec3_mul(dvec3* dest, dvec3* a, dvec3* b);
void dvec3_mul(dvec3* dest, dvec3* a, double b);
void dvec3_normalize(dvec3* dest, dvec3* a);
void dvec3_cross(dvec3* dest, dvec3* a, dvec3* b);
double dvec3_length(dvec3* a);
double dvec3_dot(dvec3* a, dvec3* b);
void dvec3_negate(dvec3* dest, dvec3* a);
void dvec3_lerp(dvec3* dest, dvec3* a, dvec3* b, double delta);

// vec4
void dvec4_clone(dvec4* dest, dvec4* src);
void dvec4_add(dvec4* dest, dvec4* a, dvec4* b);
void dvec4_add(dvec4* dest, dvec4* a, double b);
void dvec4_mul(dvec4* dest, dvec4* a, dvec4* b);
void dvec4_mul(dvec4* dest, dvec4* a, double b);
void dvec4_normalize(dvec4* dest, dvec4* a);
double dvec4_length(dvec4* a);
double dvec4_dot(dvec4* a, dvec4* b);
void dvec4_negate(dvec4* dest, dvec4* a);
void dvec4_lerp(dvec4* dest, dvec4* a, dvec3* b, double delta);

// mat3
void dmat3_identity(dmat3* mat);
void dmat3_identity(dmat3* mat, double scale);
void dmat3_mul(dvec3* dest, dvec3* left, dmat3* right);
void dmat3_mul(dmat3* dest, dmat3* left, dmat3* right);
void dmat3_rotx(dmat3* mat, double angle);
void dmat3_roty(dmat3* mat, double angle);
void dmat3_rotz(dmat3* mat, double angle);
void dmat3_rotate(dmat3* mat, double anglex, double angley, double anglez);

// mat4
void dmat4_identity(dmat4* mat);
void dmat4_identity(dmat4* mat, double scale);
void dmat4_identity(dmat4* mat, dvec3* scale);
void dmat4_mul(dvec4* dest, dvec4* left, dmat4* right);
void dmat4_mul(dmat4* dest, dmat4* left, dmat4* right);

void dmat4_rotx(dmat4* mat, double angle);
void dmat4_roty(dmat4* mat, double angle);
void dmat4_rotz(dmat4* mat, double angle);
void dmat4_rotate(dmat4* mat, double anglex, double angley, double anglez);
void dmat4_translate(dmat4* mat, double x, double y, double z);

void dmat4_ortho(dmat4* mat, double r, double l, double t, double b, double f, double n );
void dmat4_frustum(dmat4* mat, double aspect, double fov, double near_plane, double far_plane);
void dmat4_view(dmat4* mat, double x, double y, double z, double anglex, double angley);
void dmat4_view_ruf(dmat4* mat, double rx, double ry, double rz, double ux, double uy, double uz, double fx, double fy, double fz, double x, double y, double z);
void dmat4_view_ruf(dmat4* mat, dvec3* right, dvec3* up, dvec3* forward, dvec3* xyz);
void dmat4_model(dmat4* mat, double x, double y, double z, double anglex, double angley, double anglez, double scale);
void dmat4_model(dmat4* mat, double x, double y, double z, double anglex, double angley, double anglez);
//////////////////////////////////////////////

void math_toDvec2(dvec2* dest, vec2* src);
void math_toDvec3(dvec3* dest, vec3* src);
void math_toDvec4(dvec4* dest, vec4* src);
void math_toVec2(vec2* dest, dvec2* src);
void math_toVec3(vec3* dest, dvec3* src);
void math_toVec4(vec4* dest, dvec4* src);

void math_toDmat3(dmat3* dest, mat3* src);
void math_toDmat4(dmat4* dest, mat4* src);
void math_toMat3(mat3* dest, dmat3* src);
void math_toMat4(mat4* dest, dmat4* src);

#endif /* RG_VECMATH_H_ */
