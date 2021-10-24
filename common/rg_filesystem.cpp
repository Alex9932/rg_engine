/*
 * rg_filesystem.cpp
 *
 *  Created on: 4 мая 2021 г.
 *      Author: alex9932
 */

#include "rg_filesystem.h"
#include "rg_string.h"
#include <map>
#include <stdio.h>

struct _file_fs {
	rg_filesystem* filesystem;
	rg_fs_file file;
};

static std::map<Uint32, rg_filesystem*> filesystems;
static Uint32 fs_id = -1;

static void _fs_find(_file_fs* ffs, rg_string file) {
	ffs->filesystem = NULL;
	for (std::map<Uint32, rg_filesystem*>::iterator it = filesystems.begin(); it != filesystems.end(); ++it) {
		//Uint32 uid = it->first;
		rg_filesystem* filesystem = it->second;
		for (Uint32 i = 0; i < filesystem->header.files; ++i) {
			rg_fs_file f = filesystem->files[i];
			if(rg_streql(f.name, file)) {
				ffs->file = f;
				ffs->filesystem = filesystem;
				return;
			}
		}
	}
}

Uint32 rg_fs_mount(rg_string file) {
	FILE* f = fopen(file, "rb");
	if(f) {
		rg_filesystem* fs = (rg_filesystem*)rg_malloc(sizeof(rg_filesystem));
		fs->file_stream = f;
		fread(&fs->header, sizeof(rg_fs_header), 1, fs->file_stream);
		SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Mounting %s files %d", file, fs->header.files);
		fs->files = (rg_fs_file*)rg_malloc(sizeof(rg_fs_file) * fs->header.files);
		fread(fs->files, sizeof(rg_fs_file), fs->header.files, fs->file_stream);
		fs_id++;
		filesystems[fs_id] = fs;
		return fs_id;
	}
	return -1;
}

void rg_fs_umount(Uint32 fs_uid) {
	rg_filesystem* fs = filesystems[fs_uid];
	fclose(fs->file_stream);
	rg_free(fs->files);
	rg_free(fs);
	filesystems[fs_uid] = NULL;
}

rg_Resource* rg_fs_getResource(rg_string file) {
	SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "Getting: %s", file);
	_file_fs ffs;
	_fs_find(&ffs, file);
	if(ffs.filesystem == NULL) {
		SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "File: %s not found in mounted filesystems!", file);
		return NULL;
	}

	rg_fs_file f = ffs.file;
	SDL_LogInfo(SDL_LOG_CATEGORY_DEBUG, "Res: %s, off 0x%x, l %d", f.name, f.offset, f.length);
	rg_Resource* res = (rg_Resource*)rg_malloc(sizeof(rg_Resource));
	res->length = ffs.file.length;
	res->data = rg_malloc(ffs.file.length + 1);
	memset(res->data, 0, ffs.file.length + 1);
	fseek(ffs.filesystem->file_stream, ffs.file.offset, SEEK_SET);
	fread(res->data, 1, ffs.file.length, ffs.filesystem->file_stream);
	return res;
}
