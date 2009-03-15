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
