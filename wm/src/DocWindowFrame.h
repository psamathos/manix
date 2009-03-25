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

#ifndef WM_DOCWINDOWFRAME_H
#define WM_DOCWINDOWFRAME_H

#include "manix/x/Window.h"

#include "WindowFrame.h"

namespace manix {
namespace wm {

	class Label;

	class DocWindowFrame : public WindowFrame, private EventHandler {
	public:
						DocWindowFrame(
							App&					app,
							xcb_connection_t*		conn,
							xcb_window_t			parent,
							xcb_screen_t*			screen,
							ClientWindow&			client);

		void			show();

		void			moveClientTo(
							int32_t&				x,
							int32_t&				y);

		void			resizeClientTo(
							uint32_t&				width,
							uint32_t&				height);

		void			adjustClientToFrame();
		void			adjustFrameToClient();

	private:
		xcb_gcontext_t	mGc;
		xcb_window_t	mFrameParentWindow;
		x::Window		mFrameWindow;
		Label*			mWindowLabel;

		uint32_t		calcWidthFromClient();
		uint32_t		calcHeightFromClient();

		void			drawDragOutline(
							int32_t					x,
							int32_t					y);

		void			onTitleChanged(
							const std::wstring&		title);

		void			onButtonPressEvent(
							xcb_button_press_event_t*	event);
		void			onMotionNotifyEvent(
							xcb_motion_notify_event_t*	event);
		void			onButtonReleaseEvent(
							xcb_button_release_event_t*	event);
		void			onExposeEvent(
							xcb_expose_event_t*		event);
	};

} // wm
} // manix

#endif // WM_DOCWINDOWFRAME_H
