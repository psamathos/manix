#ifndef MANIX_COMMON_FONT_H
#define MANIX_COMMON_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <xcb/render.h>

namespace manix {
namespace x {
	
	class Font {
	public:
								Font(
									xcb_connection_t*			conn,
									xcb_render_pictformat_t		a8format,
									const std::string&			file,
									uint32_t					size);
		
		int16_t					getAscender() const { return mAscender; }
		int16_t					getDescender() const { return mDescender; }
		int16_t					getHeight() const { return mHeight; }
		
		uint32_t 				measure(
									const std::wstring&			string);
		void					render(
									const std::wstring&			string,
									xcb_render_picture_t		pen,
									xcb_render_picture_t		dst,
									int32_t						x,
									int32_t						y);
		
	private:
		typedef struct GlyphInfo {
			int32_t			index;
			int16_t			xOffset;
			int16_t			yOffset;
			
			GlyphInfo()
			{
				
			}
			
			GlyphInfo(int32_t idx, int16_t x, int16_t y)
				: index(idx)
				, xOffset(x)
				, yOffset(y)
			{
			}
			
		}GlyphInfo;
		
		xcb_connection_t*				mConn;
		xcb_render_picture_t			mPen;
		xcb_render_pictformat_t			mFontFormat;
		xcb_render_glyphset_t			mGlyphSet;
		std::map<wchar_t, GlyphInfo>	mCharToGlyphMap;
		FT_Face							mFtFace;
		int16_t							mAscender;
		int16_t							mDescender;
		int16_t							mHeight;
		
		void							renderGlyph(
											wchar_t						glyph);
	};
	
} // x
} // manix

#endif // MANIX_COMMON_FONT_H