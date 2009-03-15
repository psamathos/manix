#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <xcb/xcb_renderutil.h>

#include "manix/x/Font.h"


namespace manix {
namespace x {
	
	Font::Font(
		xcb_connection_t*			conn,
		xcb_render_pictformat_t	a8format,
		const std::string&		file,
		uint32_t						size)
		: mConn(conn)
		, mFontFormat(a8format)
	{
		static bool 		ft_initialized = false;
		static FT_Library	library;
		
		if(!ft_initialized)
			FT_Init_FreeType(&library);
			
		FT_New_Face(library, file.c_str(), 0, &mFtFace);
		FT_Set_Char_Size(mFtFace, 0, size * 64, 90, 90);
		
		mGlyphSet = xcb_generate_id(conn);
		xcb_render_create_glyph_set(conn, mGlyphSet, a8format);
		
		FT_Fixed ascender, descender;

 		mDescender = - FT_MulFix (mFtFace->descender, mFtFace->size->metrics.y_scale) / 64;
 		mAscender = FT_MulFix (mFtFace->ascender, mFtFace->size->metrics.y_scale) / 64;
		mHeight = FT_MulFix(mFtFace->height, mFtFace->size->metrics.y_scale) / 64;

		printf("asc:%d desc:%d height:%d\n", mAscender, mDescender, mHeight);
		
	}
	
	uint32_t
	Font::measure(
		const std::wstring&		string)
	{
		const std::wstring::const_iterator	end = string.end();

		uint32_t						penX = 0;	
		for(std::wstring::const_iterator i = string.begin(); i != end; ++i){
			uint32_t					index = FT_Get_Char_Index(mFtFace, *i);

			if(mCharToGlyphMap.count(*i) == 0)
				renderGlyph(*i);
			
			const GlyphInfo&		ginfo = mCharToGlyphMap[*i];
			
			// TODO: Add kerning here?
			
			penX += ginfo.xOffset;
		}
		
		return penX;
	}
	
	void
	Font::render(
		const std::wstring&		string,
		xcb_render_picture_t	pen,
		xcb_render_picture_t	dst,
		int32_t					x,
		int32_t					y)
	{
		 xcb_render_util_composite_text_stream_t*  stream = xcb_render_util_composite_text_stream(mGlyphSet, string.size(), 0);
         
		int32_t        lastX = 0;
		int32_t        lastY = 0;
		uint8_t        glyph;
        
		const std::wstring::const_iterator	end = string.end();
		for(std::wstring::const_iterator i = string.begin(); i != end; ++i){
			wchar_t		c = *i;
			int32_t     thisX = x;
			int32_t     thisY = y;

			if(mCharToGlyphMap.count(c) == 0)
				renderGlyph(c);
         	
			uint32_t	index = mCharToGlyphMap[c].index;
			xcb_render_util_glyphs_32(stream, thisX - lastX, thisY - lastY, 1, &index);
       	
			lastX = thisX;
			lastY = thisY;
		}
         
		xcb_render_util_composite_text(mConn, XCB_RENDER_PICT_OP_OVER, pen, dst, mFontFormat, 0, 0, stream);
		xcb_render_util_composite_text_free(stream);
	}
	
	void
	Font::renderGlyph(
		wchar_t				charcode)
	{	
		uint32_t			glyphIndex = FT_Get_Char_Index(mFtFace, charcode);
		
		FT_Load_Glyph(mFtFace, glyphIndex, FT_LOAD_RENDER);

		FT_Bitmap *bitmap=&mFtFace->glyph->bitmap;
		
		xcb_render_glyphinfo_t	ginfo;
		ginfo.x = -mFtFace->glyph->bitmap_left;
		ginfo.y = mFtFace->glyph->bitmap_top;
		ginfo.width = bitmap->width;
		ginfo.height = bitmap->rows;
		ginfo.x_off = mFtFace->glyph->advance.x/64;
		ginfo.y_off = mFtFace->glyph->advance.y/64;
		
		int		stride=(ginfo.width+3)&~3;
		uint8_t	tmpbitmap[stride*ginfo.height];	

		int y;
		for( y=0; y < ginfo.height; y++)
			memcpy(tmpbitmap+y*stride, bitmap->buffer+y*ginfo.width, ginfo.width);

		xcb_render_add_glyphs(mConn, mGlyphSet, 1, &glyphIndex, &ginfo, stride * ginfo.height, tmpbitmap);
	
		mCharToGlyphMap[charcode] = GlyphInfo(glyphIndex, ginfo.x_off, ginfo.y_off);
	}
	
	
	
} // x
} // manix