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

#ifndef WM_WINDOWFRAME_H
#define WM_WINDOWFRAME_H

#include <string>
#include <xcb/xcb.h>

namespace manix {
namespace wm {

	class App;
	class ClientWindow;

	class WindowFrame {
	public:
							WindowFrame(
								App&					app,
								xcb_connection_t*		conn,
								ClientWindow&			client);

		virtual void		show() = 0;

		virtual void		moveClientTo(
									int32_t&			x,
									int32_t&			y) = 0;

		virtual void		resizeClientTo(
									uint32_t&			width,
									uint32_t&			height) = 0;

		virtual void		adjustClientToFrame() = 0;
		virtual void		adjustFrameToClient() = 0;

		void				setTitle(
								const std::wstring&		title);
		void				getTitle(
								std::wstring&			title) { title = mTitle; }

		ClientWindow&		getClientWindow() { return mClientWindow; }

		void				beginDrag(
								xcb_window_t			window,
								xcb_button_t			button,
								int32_t					rootX,
								int32_t					rootY,
								int32_t					anchorX,
								int32_t					anchorY);
		void				continueDrag(
								int32_t					newRootX,
								int32_t					newRootY,
								int32_t&				rootX,
								int32_t&				rootY);
		bool				endDrag(
								xcb_button_t			button,
								int32_t					newRootX,
								int32_t					newRootY,
								int32_t&				rootX,
								int32_t&				rootY);
		void				getCurrentDragPos(
								int32_t&				rootX,
								int32_t&				rootY);
		bool				isDragging() const { return mIsDragging; }

	protected:

		App&				getApp() { return mApp; }

	private:
		xcb_connection_t*	mConn;
		App&				mApp;
		ClientWindow&		mClientWindow;
		std::wstring		mTitle;
		// Drag support
		bool				mIsDragging;
		xcb_window_t		mDragWindow;
		xcb_button_t		mDragButton;
		int32_t				mDragDeltaX;
		int32_t				mDragDeltaY;
		int32_t				mDragRootX;
		int32_t				mDragRootY;
		int32_t				mCurrentRootX;
		int32_t				mCurrentRootY;


		virtual void		onTitleChanged(
								const std::wstring&			newTitle);
	};

} // wm
} // manix

#endif // WM_WINDOWFRAME_H
