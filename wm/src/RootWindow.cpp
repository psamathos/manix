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
#include <cstdlib>
#include <cstring>

#include "App.h"
#include "ClientWindow.h"
#include "RootWindow.h"

namespace manix {
namespace wm {

	RootWindow::RootWindow(
		App&						app,
		xcb_connection_t*		conn,
		xcb_screen_t*			screen,
		xcb_window_t			root)
		: mApp(app)
		, mWindow(conn, root)
		, mConn(conn)
		, mScreen(screen)
	{
		setup(conn, root);
	}

	void
	RootWindow::onExposeEvent(
		xcb_expose_event_t*	event)
	{

	}

	void
	RootWindow::onConfigureRequestEvent(
		xcb_configure_request_event_t* event)
	{

		printf("configure_request on root: window=0x%08X x=%d y=%d width=%d height=%d\n",
			event->window, event->x, event->y, event->width, event->height);

		uint32_t			mask = { XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_BORDER_WIDTH };
		uint32_t			values[5] = { event->x, event->y, event->width, event->height, event->border_width };
		xcb_configure_window(mWindow.getConnection(), event->window, mask, values);
		xcb_flush(mWindow.getConnection());
	}

	void
	RootWindow::onCreateNotifyEvent(
		xcb_create_notify_event_t* event)
	{
		// We ignore this since any window that wants to get mapped will do a MapRequest at which time we'll reparent and configure them
		printf("create_notify on root; window=0x%08X x=%d y=%d width=%d height=%d\n",
			event->window, event->x, event->y, event->width, event->height);
	}

	void
	RootWindow::onMapRequestEvent(
		xcb_map_request_event_t*	event)
	{
		printf("map_request on root; window=0x%08X\n", event->window);

		xcb_connection_t*		conn = mWindow.getConnection();
		ClientWindow*			client = new ClientWindow(mApp, conn, mScreen, mWindow.getWindow(), event->window);
		client->show();


		//xcb_map_window(conn, event->window);
		xcb_flush(conn);
	}

	void
	RootWindow::setup(
		xcb_connection_t*		conn,
		xcb_window_t			root)
	{
		uint32_t					mask = XCB_CW_EVENT_MASK;
		uint32_t					values[1] = { XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_RESIZE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_EXPOSURE};

		xcb_change_window_attributes(conn, root, mask, values);
		xcb_flush(conn);

		if(xcb_connection_has_error(conn)){
			printf("Failed to grab root, will die now");
			exit(1);
		}

		mApp.addWindowHandler(root, this);
	}

} // wm
} // manix
