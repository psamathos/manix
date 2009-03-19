#include <cstdio>

#include "App.h"
#include "Label.h"

namespace manix {
namespace wm {

	Label::Label(
		App&						app,
		xcb_screen_t*				screen,
		xcb_window_t				parent,
		xcb_render_picture_t		color,
		x::Font*					font,
		HAlign						align,
		uint32_t					background,
		int32_t						x,
		int32_t						y,
		uint32_t					width,
		uint32_t					height,
		const std::wstring&		text)
		: mApp(app)
		, mWindow(app, screen, parent, x, y, width, height)
		, mText(text)
		, mFont(font)
		, mHAlign(align)
		, mHeight(height)
		, mWidth(width)
	{
		mWindow.setBackgroundColor(background);

		mWindowPict = xcb_generate_id(app);
		uint32_t		mask = XCB_RENDER_CP_POLY_EDGE | XCB_RENDER_CP_POLY_MODE;
		uint32_t		values[] = { XCB_RENDER_POLY_EDGE_SMOOTH, XCB_RENDER_POLY_MODE_IMPRECISE };

		xcb_render_create_picture(app, mWindowPict, mWindow.getWindow(), app.getDisplayFormat(), mask, values);

		app.addWindowHandler(mWindow, this);
	}

	void
	Label::setText(
		const std::wstring&			string)
	{
		mText = string;
		mTextWidth = mFont->measure(mText);
	}

	void
	Label::show()
	{
		mWindow.show();
	}

	void
	Label::onEvent(
		xcb_expose_event_t*	event)
	{
		xcb_rectangle_t		rect = { 0, 0, mWidth, mHeight };
		xcb_render_color_t	color = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
		//xcb_render_fill_rectangles(mApp, XCB_RENDER_PICT_OP_OVER, mWindowPict, color, 1, &rect);

		mApp.getSystemFont().render(mText, mApp.getBlackPen(), mWindowPict, 1, mFont->getAscender() + 1);
	}

	void
	Label::updateText()
	{
		mTextWidth = mFont->measure(mText);

	}

	void
	Label::redraw()
	{

	}

} // wm
} // manix
