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
		, mFrameWindow(conn, screen, parent, 100 + client.getX() - 20, 100 + client.getY() - 20,
				client.getWidth() + kWindowWidthDelta, client.getHeight() + kWindowHeightDelta)
	{
		mWindowLabel = new Label(app, screen, mFrameWindow, app.getBlackPen(),
				&app.getSystemFont(), Label::kHAlignLeft,
				app.getColor(kColorGray), 5, 2, 100, 18, L"");

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
		return getClientWindow().getWidth() + kWindowHeightDelta;
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
	DocWindowFrame::onEvent(
		xcb_expose_event_t*	event)
	{
		printf("window frame exposed\n");
		uint32_t			mask = XCB_GC_FOREGROUND;
		uint32_t			values[] = { getApp().getColor(kColorGray) };
		xcb_change_gc(getApp(), mGc, mask, values);

		xcb_rectangle_t		rect = { 0, 0, calcWidthFromClient(), calcHeightFromClient() };
		xcb_poly_fill_rectangle(getApp(), mFrameWindow, mGc, 1, &rect);

		//xcb_clear_area(getApp(), 0, mFrameWindow, 0, 0, calcWidthFromClient(), calcHeightFromClient());
		xcb_flush(getApp());
	}

} // wm
} // manix
