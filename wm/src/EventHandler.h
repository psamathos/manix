#ifndef WM_EVENTHANDLER_H
#define WM_EVENTHANDLER_H

namespace manix {
namespace wm {

	class EventHandler {
	public:
		virtual void		onExposeEvent(
								xcb_expose_event_t*				event){}
		virtual void		onConfigureRequestEvent(
								xcb_configure_request_event_t*	event){}
		virtual void		onCreateNotifyEvent(
								xcb_create_notify_event_t*		event){}
		virtual void		onMapRequestEvent(
								xcb_map_request_event_t*		event){}
		virtual void		onButtonPressEvent(
								xcb_button_press_event_t*		event){}
		virtual void		onButtonReleaseEvent(
								xcb_button_release_event_t*		event){}
	};

} // wm
} // manix

#endif // WM_EVENTHANDLER_H
