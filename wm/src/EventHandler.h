#ifndef WM_EVENTHANDLER_H
#define WM_EVENTHANDLER_H

namespace manix {
namespace wm {
	
	class EventHandler {
	public:
		virtual void		onEvent(
									xcb_expose_event_t*	event){}
		virtual void		onEvent(
									xcb_configure_request_event_t*	event){}
		virtual void		onEvent(
									xcb_create_notify_event_t*			event){}
		virtual void		onEvent(
									xcb_map_request_event_t*			event){}
	};
		
} // wm
} // manix

#endif // WM_EVENTHANDLER_H