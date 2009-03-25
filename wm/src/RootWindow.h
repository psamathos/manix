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

#ifndef WM_ROOTWINDOW_H
#define WM_ROOTWINDOW_H

#include <map>
#include <xcb/xcb.h>

#include "manix/x/Window.h"

#include "EventHandler.h"

namespace manix {
namespace wm {

	class App;

	class RootWindow : public EventHandler {
	public:
							RootWindow(
								App&					app,
								xcb_connection_t*	conn,
								xcb_screen_t*		screen,
								xcb_window_t		root);

		void				onExposeEvent(
								xcb_expose_event_t*	event);
		void				onConfigureRequestEvent(
								xcb_configure_request_event_t*	event);
		void				onCreateNotifyEvent(
								xcb_create_notify_event_t*			event);
		void				onMapRequestEvent(
								xcb_map_request_event_t*			event);

	private:
		App&				mApp;
		x::Window		mWindow;
		xcb_connection_t*	mConn;
		xcb_screen_t*	mScreen;

		void				setup(
								xcb_connection_t*	conn,
								xcb_window_t		root);
	};

} // wm
} // manix

#endif // WM_ROOTWINDOW_H
