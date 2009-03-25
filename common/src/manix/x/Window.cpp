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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "manix/x/Window.h"

namespace manix {
namespace x {

	Window::Window(
		xcb_connection_t*	connection,
		xcb_screen_t*		screen,
		xcb_window_t		parent,
		uint32_t			eventMask,
		uint32_t			x,
		uint32_t			y,
		uint32_t			width,
		uint32_t			height)
		: mConn(connection)
	{
		assert(mConn != NULL);

		mWindow = xcb_generate_id(connection);

		uint32_t	mask = XCB_CW_EVENT_MASK;
		uint32_t	values[] = { eventMask };
		xcb_create_window(
			connection,
			screen->root_depth,
			mWindow,
			parent,
 			x, y, width, height,
			0, // border
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			screen->root_visual,
			mask, values);
	}

	Window::Window(
		xcb_connection_t*	connection,
		xcb_window_t		window)
		: mConn(connection)
		, mWindow(window)
	{
		assert(mConn != NULL);
	}

	void
	Window::getGeometry(
		uint32_t&			width,
		uint32_t&			height,
		int32_t&			x,
		int32_t&			y)
	{
		xcb_get_geometry_cookie_t		cookie = xcb_get_geometry(mConn, mWindow);

		xcb_generic_error_t*			error;
		xcb_get_geometry_reply_t*		reply = xcb_get_geometry_reply(mConn, cookie, &error);

		width = reply->width;
		height = reply->height;
		x = reply->x;
		y = reply->y;
	}

	void
	Window::reparent(
		x::Window*			parent,
		int32_t				x,
		int32_t				y)
	{
		xcb_reparent_window(mConn, mWindow, parent->mWindow, x, y);
	}

	void
	Window::setBackgroundColor(
		uint32_t			background)
	{
		uint32_t			mask = XCB_CW_BACK_PIXEL;
		uint32_t			values[] = { background };
		xcb_change_window_attributes(mConn, mWindow, mask, values);
	}

	void
	Window::setBackground(
		xcb_pixmap_t		pixmap)
	{
		uint32_t			mask = XCB_CW_BACK_PIXMAP;
		uint32_t			values[1] = { pixmap };
		xcb_change_window_attributes(mConn, mWindow, mask, values);
	}

	void
	Window::moveTo(
		int32_t				x,
		int32_t				y)
	{
		uint32_t			mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		int32_t				values[] = { x, y };

		xcb_configure_window(mConn, mWindow, mask, (uint32_t*) values);
	}

	void
	Window::resizeTo(
		uint32_t			width,
		uint32_t			height)
	{
		uint32_t			mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
		uint32_t			values[] = { width, height };

		xcb_configure_window(mConn, mWindow, mask, values);
	}

	void
	Window::show()
	{
		xcb_map_window(mConn, mWindow);
	}

	void
	Window::showWithSubs()
	{
		xcb_map_window(mConn, mWindow);
		xcb_map_subwindows(mConn, mWindow);
	}

} // x
} // manix
