/*
 * xr_geom.cpp
 *
 *  Created on: 18 апр. 2021 г.
 *      Author: alex9932
 */

#include "xr_geom.h"
#include <stdio.h>

void xr_read_geom(rg_string file) {
	FILE* f = fopen(file, "rb");

	xr_geom_version ver;
	xr_vertex vtx;

	xr_vertex_block vtx_;

	fread(&ver, sizeof(xr_geom_version), 1, f);
	fread(&vtx, sizeof(xr_vertex), 1, f);

	printf("xrGeom %s\n", file);
	printf("VERSION BLOCK\n");
	printf("block: %d\n", ver.block_code);
	printf("is compress: %d\n", ver.compression);
	printf("block size: %d\n", ver.block_size);
	printf("version: %d\n", ver.version);

	printf("\nVERTICES BLOCK\n");
	printf("block: %d\n", vtx.block_code);
	printf("is compress: %d\n", vtx.compression);
	printf("block size: %d\n", vtx.block_size);
	printf("blocks count: %d\n", vtx.blocks_count);

	fread(&vtx_, sizeof(xr_vertex_block), 1, f);

	printf("\nVERTICES DEFINE BLOCK\n");
	printf("start of new block: %d\n", vtx_.new_block);
	printf("start vertex format: %d\n", vtx_.start_vertex_format);

	printf("\nVERTICES FORMAT\n");
	printf("ZERO: %d\n", vtx_.vertex_format.ZERO);
	printf("offset: %d\n", vtx_.vertex_format.offset);
	printf("type: %d\n", vtx_.vertex_format.var_type);
	printf("var: %d\n\n", vtx_.vertex_format.var);

	printf("end vertex format: %d\n", vtx_.end_vertex_format);
	printf("start vertices: %d\n", vtx_.start_vertices);
	printf("vertices: %d\n", vtx_.vertex_count);


	xr_brush* data = (xr_brush*)rg_malloc(vtx.block_size);
	fread(data, 1, vtx.block_size, f);

	printf("b_size %d, brush %d", vtx.block_size / vtx_.vertex_count, sizeof(xr_brush));





	fclose(f);
}
