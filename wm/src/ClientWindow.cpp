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
#include "ClientWindow.h"
#include "DocWindowFrame.h"

namespace manix {
namespace wm {

	ClientWindow::ClientWindow(
		App&					app,
		xcb_connection_t*		conn,
		xcb_screen_t*			screen,
		xcb_window_t			parent,
		xcb_window_t			client)
		: mApp(app)
		, mClientWindow(conn, client)
	{
		printf("Creating new client window\n");

		mClientWindow.getGeometry(mWidth, mHeight, mX, mY);

		mWindowFrame = new DocWindowFrame(app, conn, screen->root, screen, *this);
	}

	void
	ClientWindow::reparent(
		x::Window*				window,
		int32_t					x,
		int32_t					y)
	{
		mClientWindow.reparent(window, x, y);
	}

	void
	ClientWindow::show()
	{
		mWindowFrame->show();
	}

} // wm
} // manix
