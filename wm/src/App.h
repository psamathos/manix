#ifndef WM_APP_H
#define WM_APP_H

#include <xcb/xcb.h>
#include <xcb/render.h>

#include "manix/x/AtomCache.h"
#include "manix/x/Font.h"
#include "manix/x/Window.h"

#include "Types.h"

namespace manix {
namespace wm {
	
	class App {
	public:
										App(
											xcb_connection_t*			conn,
											xcb_screen_t*				screen,
											x::Font&						systemFont,
											x::AtomCache&				atomCache,
											xcb_render_pictformat_t	formatA8,
											xcb_render_pictformat_t	formatRGB24,
											xcb_render_pictformat_t	formatARGB32,
											xcb_render_picture_t		blackPen);
										~App();
		
		void							addWindowHandler(
											xcb_window_t		window,
											EventHandler*		handler);
		
		x::Font&						getSystemFont() { return mSystemFont; }

		xcb_render_pictformat_t	getPictFormatA8() const { return mFormatA8; }		
		xcb_render_pictformat_t	getPictFormatRGB24() const { return mFormatRGB24; }
		xcb_render_pictformat_t	getPictFormatARGB32() const { return mFormatARGB32; }
		
		xcb_render_pictformat_t	getDisplayFormat() const { return mFormatRGB24; }

		
		xcb_render_picture_t		getBlackPen() const { return mBlackPen; }
		
							operator xcb_connection_t* () { return mConn; }
		
		void							run();
	
	private:
		xcb_connection_t*			mConn;
		xcb_screen_t*				mScreen;
		WindowMap					mWindowMap;
		x::Font						mSystemFont;
		x::AtomCache				mAtomCache;
		xcb_render_pictformat_t mFormatA8;
		xcb_render_pictformat_t	mFormatRGB24;
		xcb_render_pictformat_t	mFormatARGB32;
		xcb_render_picture_t		mBlackPen;
		
		template <typename EventType>
		void
		dispatchEventByWindow(EventType* event)
		{
			if(mWindowMap.count(event->window) == 0){
				printf("Got event for unknown window 0x%08X\n", event->window);
			}else{
				mWindowMap[event->window]->onEvent(event);
			}
		}

		template <typename EventType>
		void
		dispatchEventByParent(EventType* event)
		{
			if(mWindowMap.count(event->parent) == 0){
				printf("Got event for unknown window 0x%08X\n", event->parent);
			}else{
				mWindowMap[event->parent]->onEvent(event);
			}
		}
	};
	
} // wm
} // manix

#endif // WM_APP_H