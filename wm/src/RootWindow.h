#ifndef WM_ROOTWINDOW_H
#define WM_ROOTWINDOW_H

#include <map>
#include <xcb/xcb.h>

#include "manix/x/Window.h"

#include "EventHandler.h"

namespace manix {
namespace wm {
	
	class App;
	
	class RootWindow : public EventHandler {
	public:
							RootWindow(
								App&					app,
								xcb_connection_t*	conn,
								xcb_screen_t*		screen,
								xcb_window_t		root);
		
		void				onEvent(
								xcb_expose_event_t*	event);
		void				onEvent(
								xcb_configure_request_event_t*	event);
		void				onEvent(
								xcb_create_notify_event_t*			event);
		void				onEvent(
								xcb_map_request_event_t*			event);
			
	private:
		App&				mApp;
		x::Window		mWindow;
		xcb_connection_t*	mConn;
		xcb_screen_t*	mScreen;
		
		void				setup(
								xcb_connection_t*	conn,
								xcb_window_t		root);
	};
	
} // wm	
} // manix

#endif // WM_ROOTWINDOW_H