/*
 * Copyright (c) 2009 Magnus Sjöstrand <magnus@manix.nu>
 *
 * This file is part of manix.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
		const std::wstring&			text)
		: mApp(app)
		, mWindow(app, screen, parent, XCB_EVENT_MASK_EXPOSURE, x, y, width, height)
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
	Label::onExposeEvent(
		xcb_expose_event_t*	event)
	{
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
