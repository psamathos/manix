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
#include <xcb/xcb.h>

#include "App.h"
#include "ClientWindow.h"
#include "DocWindowFrame.h"
#include "Label.h"

namespace manix {
namespace wm {

	namespace {
		const uint32_t		kWindowWidthDelta = 17;
		const uint32_t		kWindowHeightDelta = 32;
		const int32_t		kWindowPosLeft = 10;
		const int32_t		kWindowPosTop = 24;
	}

	DocWindowFrame::DocWindowFrame(
		App&					app,
		xcb_connection_t*		conn,
		xcb_window_t			parent,
		xcb_screen_t*			screen,
		ClientWindow&			client)
		: WindowFrame(app, conn, client)
		, mFrameWindow(
				conn, screen, parent,
				XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS,
				100 + client.getX() - 20, 100 + client.getY() - 20,
				client.getWidth() + kWindowWidthDelta,
				client.getHeight() + kWindowHeightDelta)
	{
		mWindowLabel = new Label(app, screen, mFrameWindow, app.getBlackPen(),
				&app.getSystemFont(), Label::kHAlignLeft,
				app.getColor(kColorGray), 5, 2, 100, 18, L"");

		mFrameParentWindow = screen->root;

		mGc = xcb_generate_id(app);
		uint32_t			mask = XCB_GC_FOREGROUND;
		uint32_t			values[] = { app.getColor(kColorGray) };
		xcb_create_gc(app, mGc, mFrameWindow, mask, values);

		app.addWindowHandler(mFrameWindow, this);

		// Reparent last to make the client window topmost
		client.reparent(&mFrameWindow, kWindowPosLeft, kWindowPosTop);
		setTitle(L"Test title");
	}

	void
	DocWindowFrame::show()
	{
		mFrameWindow.showWithSubs();
	}

	void
	DocWindowFrame::moveClientTo(
		int32_t&					x,
		int32_t&					y)
	{

	}

	void
	DocWindowFrame::resizeClientTo(
		uint32_t&				width,
		uint32_t&				height)
	{

	}

	void
	DocWindowFrame::adjustClientToFrame()
	{

	}

	void
	DocWindowFrame::adjustFrameToClient()
	{

	}

	uint32_t
	DocWindowFrame::calcWidthFromClient()
	{
		return getClientWindow().getWidth() + kWindowWidthDelta;
	}

	uint32_t
	DocWindowFrame::calcHeightFromClient()
	{
		return getClientWindow().getHeight() + kWindowHeightDelta;
	}

	void
	DocWindowFrame::drawDragOutline(
		int32_t					x,
		int32_t					y)
	{
		uint32_t				mask = XCB_GC_FUNCTION | XCB_GC_LINE_WIDTH | XCB_GC_SUBWINDOW_MODE;
		uint32_t				values[] = {XCB_GX_XOR, 2, XCB_SUBWINDOW_MODE_INCLUDE_INFERIORS };

		xcb_rectangle_t			rect;
		rect.x = x;
		rect.y = y;
		rect.width = calcWidthFromClient();
		rect.height = calcHeightFromClient();

		xcb_change_gc(getApp(), mGc, mask, values);
		xcb_poly_rectangle(getApp(), mFrameParentWindow, mGc, 1, &rect);
	}

	/**
	 *
	 */
	void
	DocWindowFrame::onTitleChanged(
		const std::wstring&		newTitle)
	{
		mWindowLabel->setText(newTitle);

		uint32_t			textWidth = mWindowLabel->getTextWidth();

		mWindowLabel->resizeTo(textWidth + 5, 17);
		mWindowLabel->moveTo((calcWidthFromClient() - textWidth) / 2, 2);
	}

	void
	DocWindowFrame::onButtonPressEvent(
		xcb_button_press_event_t*	event)
	{
		printf("Beginning drag\n");

		beginDrag(mFrameWindow, event->detail,
				event->root_x, event->root_y,
				event->event_x, event->event_y);

		int32_t			x, y;
		getCurrentDragPos(x, y);
		drawDragOutline(x, y);

	}

	void
	DocWindowFrame::onMotionNotifyEvent(
		xcb_motion_notify_event_t*	event)
	{
		if(isDragging()){
			int32_t			x, y;
			getCurrentDragPos(x, y);
			drawDragOutline(x, y);

			continueDrag(event->root_x, event->root_y, x, y);

			drawDragOutline(x, y);
		}
	}

	void
	DocWindowFrame::onButtonReleaseEvent(
		xcb_button_release_event_t*	event)
	{
		if(isDragging()){
			int32_t			prevX, prevY;
			getCurrentDragPos(prevX, prevY);

			int32_t			x, y;
			if(endDrag(event->detail, event->root_x, event->root_y, x, y)){
				drawDragOutline(prevX, prevY);

				mFrameWindow.moveTo(x, y);
			}
		}
	}

	void
	DocWindowFrame::onExposeEvent(
		xcb_expose_event_t*		event)
	{
		uint32_t			mask = XCB_GC_FUNCTION | XCB_GC_FOREGROUND
									| XCB_GC_FILL_STYLE | XCB_GC_STIPPLE
									| XCB_GC_SUBWINDOW_MODE;
		uint32_t			values[] = { XCB_GX_COPY, getApp().getColor(kColorGray),
									XCB_FILL_STYLE_STIPPLED, getApp().getGrayStipple(),
									XCB_SUBWINDOW_MODE_CLIP_BY_CHILDREN };
		xcb_change_gc(getApp(), mGc, mask, values);

		xcb_rectangle_t		rect = { 0, 0, calcWidthFromClient(), calcHeightFromClient() };
		xcb_poly_fill_rectangle(getApp(), mFrameWindow, mGc, 1, &rect);
	}

} // wm
} // manix
