/*
 * xr_geom.h
 *
 * http://stalkerin.gameru.net/wiki/index.php?title=Level.geom
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef XR_GEOM_H_
#define XR_GEOM_H_

#include <stdint.h>
#include "rg_engine.h"

typedef uint8_t BYTE_t;
typedef uint16_t WORD_t;
typedef uint32_t DWORD_t;
typedef uint64_t QWORD_t;
typedef uint16_t SHORTSINGLE_t;
typedef uint32_t SINGLE_t;
typedef uint64_t DOUBLE_t;

enum xr_geom_block {
	XR_BLOCK_COMPILER = 1,
	XR_BLOCK_VERTICES = 9,
	XR_BLOCK_INDICES = 10,
	XR_BLOCK_SYNC = 11
};

typedef struct xr_geom_version {
	WORD_t block_code;
	WORD_t compression;
	DWORD_t block_size;
	DWORD_t version;
} xr_geom_version;

typedef struct xr_vertex {
	WORD_t block_code;
	WORD_t compression;
	DWORD_t block_size;
	DWORD_t blocks_count;
} xr_vertex;

typedef struct xr_vertex_format {
	WORD_t ZERO;
	WORD_t offset;
	WORD_t var_type;
	WORD_t var;
} xr_vertex_format;

typedef struct xr_vertex_block {
	DWORD_t new_block;
	DWORD_t start_vertex_format;
	xr_vertex_format vertex_format;
	DWORD_t end_vertex_format;
	DWORD_t start_vertices;
	DWORD_t vertex_count;
} xr_vertex_block;

typedef struct xr_brush {
	DWORD_t x;
	DWORD_t y;
	DWORD_t z;
	BYTE_t normx;
	BYTE_t normy;
	BYTE_t normz;
	BYTE_t light_factor;
	BYTE_t tanx;
	BYTE_t tany;
	BYTE_t tanz;
	BYTE_t tex_coorect_x;
	BYTE_t btanx;
	BYTE_t btany;
	BYTE_t btanz;
	BYTE_t tex_coorect_y;
	SINGLE_t texx;
	SINGLE_t texy;
	SHORTSINGLE_t lmx;
	SHORTSINGLE_t lmy;
} xr_brush;

void xr_read_geom(rg_string file);

#endif /* XR_GEOM_H_ */
