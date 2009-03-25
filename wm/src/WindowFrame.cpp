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
#include <cstdlib>

#include "WindowFrame.h"

namespace manix {
namespace wm {

	WindowFrame::WindowFrame(
		App&					app,
		xcb_connection_t*		conn,
		ClientWindow&			client)
		: mConn(conn)
		, mApp(app)
		, mClientWindow(client)
		, mIsDragging(false)
	{

	}

	void
	WindowFrame::setTitle(
		const std::wstring&		title)
	{
		if(title != mTitle){
			mTitle = title;
			onTitleChanged(mTitle);
		}
	}

	void
	WindowFrame::beginDrag(
		xcb_window_t			window,
		xcb_button_t			button,
		int32_t					rootX,
		int32_t					rootY,
		int32_t					anchorX,
		int32_t					anchorY)
	{
		assert(mIsDragging == false);

		mIsDragging = true;

		mDragWindow = window;
		mDragButton = button;

		mDragRootX = rootX;
		mDragRootY = rootY;

		mDragDeltaX = mCurrentRootX = rootX - anchorX;
		mDragDeltaY = mCurrentRootY = rootY - anchorY;

		xcb_grab_pointer_cookie_t	c = xcb_grab_pointer(mConn, false, window,
				XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION,
				XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_SYNC, XCB_NONE, XCB_NONE, XCB_CURRENT_TIME);

		xcb_generic_error_t*	error;
		xcb_grab_pointer_reply_t*	r = xcb_grab_pointer_reply(mConn, c, &error);
		if(error){
			printf("WARNING: error when grabbing pointer; error_code=%d\n", error->error_code);
		}

		free(r);
	}

	void
	WindowFrame::continueDrag(
		int32_t					newRootX,
		int32_t					newRootY,
		int32_t&				rootX,
		int32_t&				rootY)
	{
		assert(mIsDragging == true);

		rootX = mCurrentRootX = mDragDeltaX + (newRootX - mDragRootX);
		rootY = mCurrentRootY = mDragDeltaY + (newRootY - mDragRootY);
	}

	bool
	WindowFrame::endDrag(
		xcb_button_t			button,
		int32_t					newRootX,
		int32_t					newRootY,
		int32_t&				rootX,
		int32_t&				rootY)
	{
		if(button != mDragButton)
			return false;

		mIsDragging = false;

		rootX = mCurrentRootX = mDragDeltaX + (newRootX - mDragRootX);
		rootY = mCurrentRootY = mDragDeltaY + (newRootY - mDragRootY);

		xcb_ungrab_pointer(mConn, XCB_CURRENT_TIME);

		return true;
	}

	void
	WindowFrame::getCurrentDragPos(
		int32_t&				rootX,
		int32_t&				rootY)
	{
		rootX = mCurrentRootX;
		rootY = mCurrentRootY;
	}

	void
	WindowFrame::onTitleChanged(
		const std::wstring&		newTitle)
	{

	}

} // wm
} // manix
