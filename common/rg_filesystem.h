/*
 * rg_filesystem.h
 *
 *  Created on: 4 мая 2021 г.
 *      Author: alex9932
 */

#ifndef RG_FILESYSTEM_H_
#define RG_FILESYSTEM_H_

#include <SDL2/SDL.h>
#include "rg_engine.h"

/**

~~~~~~~~~~~~~ READ  ONLY ~~~~~~~~~~~~~
 ! for write .rfs file use rg_tools !
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<fs_header>
	RFS0
	2
<fs_file>
	texture/art_ds.png
	0x[start_position]
	0x[length]
<fs_file>
	texture/art_ff.png
	0x[start_position]
	0x[length]
<data>

*/

typedef struct rg_fs_header {
	char magic[4];
	Uint32 files;
} rg_fs_header;

typedef struct rg_fs_file {
	char name[256];
	Uint32 offset;
	Uint32 length;
} rg_fs_file;

typedef struct rg_filesystem {
	FILE* file_stream;
	rg_fs_header header;
	rg_fs_file* files;
} rg_filesystem;

Uint32 rg_fs_mount(rg_string file); // Return unique id of mounted file system.
void rg_fs_umount(Uint32 fs_uid);
rg_Resource* rg_fs_getResource(rg_string file);

#endif /* RG_FILESYSTEM_H_ */
