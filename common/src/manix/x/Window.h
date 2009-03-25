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

#ifndef MANIX_COMMON_WINDOW_H
#define MANIX_COMMON_WINDOW_H

#include <xcb/xcb.h>
#include <stdint.h>

namespace manix {
namespace x {

	class Window {
	public:
							Window(
								xcb_connection_t*	connection,
								xcb_screen_t*		screen,
								xcb_window_t		parent,
								uint32_t			eventMask,
								uint32_t			x,
								uint32_t			y,
								uint32_t			width,
								uint32_t			height);

							Window(
								xcb_connection_t*	connection,
								xcb_window_t		window);

		void				getGeometry(
								uint32_t&			width,
								uint32_t&			height,
								int32_t&			x,
								int32_t&			y);

		void				reparent(
								x::Window*			parent,
								int32_t				x,
								int32_t				y);

		void				setBackgroundColor(
								uint32_t			color);
		void				setBackground(
								xcb_pixmap_t		pixmap);

		void				moveTo(
								int32_t				x,
								int32_t				y);

		void				resizeTo(
								uint32_t			width,
								uint32_t			height);

		void				show();
		void				showWithSubs();

		xcb_window_t		getWindow() const { return mWindow; }
		xcb_connection_t*	getConnection() const { return mConn; }

							operator xcb_window_t () { return mWindow; }

	private:
		xcb_connection_t*	mConn;
		xcb_window_t		mWindow;
	};

} // x
} // manix

#endif // MANIX_COMMON_WINDOW_H
