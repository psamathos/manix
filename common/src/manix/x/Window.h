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
