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
		virtual void		onMotionNotifyEvent(
								xcb_motion_notify_event_t*		event){}
	};

} // wm
} // manix

#endif // WM_EVENTHANDLER_H
