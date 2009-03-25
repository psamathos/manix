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

#ifndef CLIENT_WINDOW_H
#define CLIENT_WINDOW_H

#include "manix/x/Window.h"

#include "Types.h"
#include "EventHandler.h"

namespace manix {
namespace wm {

	class App;
	class WindowFrame;

	class ClientWindow : public EventHandler {
	public:
						ClientWindow(
							App&					app,
							xcb_connection_t*		conn,
							xcb_screen_t*			screen,
							xcb_window_t			parent,
							xcb_window_t			window);

		void			reparent(
							x::Window*				window,
							int32_t					x,
							int32_t					y);

		void			show();

		uint32_t		getWidth() const { return mWidth; }
		uint32_t		getHeight() const { return mHeight; }
		int32_t			getX() const { return mX; }
		int32_t			getY() const { return mY; }

	private:
		App&			mApp;
		x::Window		mClientWindow;
		uint32_t		mWidth;
		uint32_t		mHeight;
		int32_t			mX;
		int32_t			mY;
		WindowFrame*	mWindowFrame;
	};

} // wm
} // manix

#endif // CLIENT_WINDOW_H
