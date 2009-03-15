#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#include <map>

#include "manix/x/AtomCache.h"
#include "manix/x/Window.h"

#include "App.h"
#include "RootWindow.h"

namespace manix {
namespace wm {
	
	App::App(
		xcb_connection_t*			conn,
		xcb_screen_t*				screen,
		x::Font&						systemFont,
		x::AtomCache&				atomCache,
		xcb_render_pictformat_t	formatA8,
		xcb_render_pictformat_t	formatRGB24,
		xcb_render_pictformat_t	formatARGB32,
		xcb_render_picture_t		blackPen)
		: mConn(conn)
		, mScreen(screen)
		, mSystemFont(systemFont)
		, mAtomCache(atomCache)
		, mFormatA8(formatA8)
		, mFormatRGB24(formatRGB24)
		, mFormatARGB32(formatARGB32)
		, mBlackPen(blackPen)
	{
		
	}
	
	App::~App()
	{
		
	}
	
	void
	App::addWindowHandler(
		xcb_window_t			window,
		EventHandler*			handler)
	{
		mWindowMap[window] = handler;
	}
	
	void
	App::run()
	{	
		bool				done = false;
		xcb_generic_event_t*	event;
		while(!done && (event = xcb_wait_for_event(mConn))){
			
			switch(event->response_type & ~0x80){
				case XCB_EXPOSE:
					dispatchEventByWindow((xcb_expose_event_t*) event);
					break;
				
				case XCB_CREATE_NOTIFY:
					dispatchEventByParent((xcb_create_notify_event_t*) event);
					break;
					
				case XCB_CONFIGURE_REQUEST:
					dispatchEventByParent((xcb_configure_request_event_t*) event);
					break;
					
				case XCB_MAP_REQUEST:
					dispatchEventByParent((xcb_map_request_event_t*) event);
					break;

				case XCB_KEY_PRESS:
					done = true;
					break;

				default:
					printf("WARNING: Unhandled event; type = %d\n", event->response_type);
					break;
			}
			
			xcb_flush(mConn);
			
			free(event);
		}

	}
		
} // wm
} // manix

namespace {
	
	void
	queryPictureFormats(
		xcb_connection_t*				conn,
		xcb_render_pictformat_t&	a8,
		xcb_render_pictformat_t&	argb32,
		xcb_render_pictformat_t&	rgb24)
	{
		xcb_render_query_pict_formats_cookie_t		cookie = xcb_render_query_pict_formats(conn);
		
		xcb_generic_error_t*								error;
		xcb_render_query_pict_formats_reply_t*		formats = xcb_render_query_pict_formats_reply(conn, cookie, &error);
		
		xcb_render_pictforminfo_iterator_t			i = xcb_render_query_pict_formats_formats_iterator(formats);
		
		printf("XRender; available formats\n");
		for(int32_t count = 0; i.rem != 0; count ++){
			xcb_render_pictforminfo_next(&i);
			
			if(i.data->depth == 32 && i.data->direct.red_mask == 0xFF
			 		&& i.data->direct.green_mask == 0xFF && i.data->direct.blue_mask == 0xFF
					&& i.data->direct.alpha_mask == 0xFF){
				printf("*");
 				argb32 = i.data->id;
			}else if(i.data->depth == 24 && i.data->direct.red_mask == 0xFF
				 	&& i.data->direct.green_mask == 0xFF && i.data->direct.blue_mask == 0xFF
					&& i.data->direct.alpha_mask == 0x00){
				printf("*");
 				rgb24 = i.data->id;
			}else if(i.data->depth == 8 && i.data->direct.alpha_mask == 0xFF){
				printf("*");
 				a8 = i.data->id;
			}
			
			printf("depth:%d r:%04X g:%04X b: %04X a:%04X\n", 
				i.data->depth, i.data->direct.red_mask, i.data->direct.green_mask, i.data->direct.blue_mask, i.data->direct.alpha_mask);
		}
	}
	
	xcb_render_picture_t
	createPen(
		xcb_connection_t*					conn,
		xcb_window_t						penParentWindow,
		xcb_render_pictformat_t			argb32format,
		const xcb_render_color_t&		color)
	{
		xcb_pixmap_t			pxId = xcb_generate_id(conn);
		xcb_create_pixmap(conn, 32, pxId, penParentWindow, 1, 1);

		xcb_render_picture_t	pen = xcb_generate_id(conn);
		uint32_t					mask = XCB_RENDER_CP_REPEAT;
		uint32_t					values[] = { XCB_RENDER_REPEAT_NORMAL };
		xcb_render_create_picture(conn, pen, pxId, argb32format, mask, values);
		
		xcb_rectangle_t		rect = { 0, 0, 1, 1 };
		xcb_render_fill_rectangles(conn, XCB_RENDER_PICT_OP_OVER, pen, color, 1, &rect);
		
		xcb_free_pixmap(conn, pxId);
		
		return pen;
	}
}

using namespace manix;

int32_t
main(int32_t argc, char* argv[])
{
	xcb_connection_t*	conn = NULL;
 	conn = xcb_connect(NULL, NULL);
	
	if(xcb_connection_has_error(conn)){
		printf("Failed to open display\n");
		exit(1);
	}	
	
	// Create resources needed by the app
	xcb_screen_t*	screen =	xcb_setup_roots_iterator( xcb_get_setup(conn) ).data;
	
	// Locate formats we need
	xcb_render_pictformat_t		formatA8;
	xcb_render_pictformat_t		formatARGB32;
	xcb_render_pictformat_t		formatRGB24;
	queryPictureFormats(conn, formatA8, formatARGB32, formatRGB24);
	
	// Create pens we'll need
	xcb_render_color_t			black = { 0, 0, 0, 0xFFFF };
	xcb_render_picture_t			blackPen = createPen(conn, screen->root, formatARGB32, black);
	
	// Create fonts needed
	x::Font					systemFont(conn, formatA8, "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraBd.ttf", 12);
	
	// Everyone needs an atom cache
	x::AtomCache			atomCache(conn);
	
	wm::App					app(conn, screen, systemFont, atomCache, formatA8, formatRGB24, formatARGB32, blackPen);
	
	wm::RootWindow*		rootWindow = new wm::RootWindow(app, conn, screen, screen->root);
	
	app.run();
	
	xcb_disconnect(conn);
	
	return 0;
}
