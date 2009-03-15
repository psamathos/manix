#ifndef WM_TYPES_H
#define WM_TYPES_H

#include <map>
#include <xcb/xcb.h>

#include "EventHandler.h"

namespace manix {
namespace wm {
		
	typedef std::map<xcb_window_t, EventHandler*>	WindowMap;
		
} // wm
} // manix

#endif // WM_TYPES_H