/*
 * cl_font.cpp
 *
 *  Created on: 27 апр. 2021 г.
 *      Author: alex9932
 */

#include "cl_font.h"
#include <rg_loader.h>

#include <ft2build.h>
#include FT_FREETYPE_H

static FT_Library ft;

void cl_font_init() {
	if (FT_Init_FreeType(&ft)) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not init freetype library!");
		return;
	}
}

void cl_font_destroy() {
	FT_Done_FreeType(ft);
}

cl_font_t* cl_font_new(rg_string file, Uint32 size) {
	cl_font_t* font = (cl_font_t*)rg_malloc(sizeof(cl_font_t));
	font->f_size = size;
	FT_Face face;
	rg_Resource* f_res = rg_loadResource(file);
	//if (FT_New_Face(ft, file, 0, &face)) {
	if (FT_New_Memory_Face(ft, (const FT_Byte*)f_res->data, f_res->length, 0, &face)) {
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not open font!");
	}
	FT_Set_Pixel_Sizes(face, 0, font->f_size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &font->atlas);
	glBindTexture(GL_TEXTURE_2D, font->atlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->f_size * F_ATLAS_WIDTH, font->f_size * F_ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (wchar_t i = 0; i < F_ATLAS_WIDTH * F_ATLAS_HEIGHT; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to load Glyph!");
		}

		font->chars[i].advance_x = face->glyph->advance.x;
		font->chars[i].advance_y = face->glyph->advance.y;
		font->chars[i].bearing_x = face->glyph->bitmap_left;
		font->chars[i].bearing_y = face->glyph->bitmap_top;
		font->chars[i].size_x = face->glyph->bitmap.width;
		font->chars[i].size_y = face->glyph->bitmap.rows;

		int xoffset = i % F_ATLAS_WIDTH;
		int yoffset = i / F_ATLAS_WIDTH;

		glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset * font->f_size, yoffset * font->f_size,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	}
	FT_Done_Face(face);
	rg_freeResource(f_res);

	return font;
}

void cl_font_delete(cl_font_t* font) {
	rg_free(font);
}
