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
