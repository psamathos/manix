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
		x::Font&					systemFont,
		x::AtomCache&				atomCache,
		xcb_render_pictformat_t		formatA8,
		xcb_render_pictformat_t		formatRGB24,
		xcb_render_pictformat_t		formatARGB32,
		xcb_render_picture_t		blackPen,
		uint32_t					colors[])
		: mConn(conn)
		, mScreen(screen)
		, mSystemFont(systemFont)
		, mAtomCache(atomCache)
		, mFormatA8(formatA8)
		, mFormatRGB24(formatRGB24)
		, mFormatARGB32(formatARGB32)
		, mBlackPen(blackPen)
	{
		memcpy(mColors, colors, sizeof(mColors));
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
			EventHandler*		handler = NULL;

			switch(event->response_type & 0x7F){

				// Target is 'window'
				case XCB_EXPOSE:
					if((handler = findHandler(reinterpret_cast<xcb_expose_event_t*>(event)->window)) != NULL)
						handler->onExposeEvent(reinterpret_cast<xcb_expose_event_t*>(event));
					break;

				// Target is 'parent'
				case XCB_MAP_REQUEST:
					if((handler = findHandler(reinterpret_cast<xcb_map_request_event_t*>(event)->parent)) != NULL)
						handler->onMapRequestEvent(reinterpret_cast<xcb_map_request_event_t*>(event));
					break;

				case XCB_CONFIGURE_REQUEST:
					if((handler = findHandler(reinterpret_cast<xcb_configure_request_event_t*>(event)->parent)) != NULL)
						handler->onConfigureRequestEvent(reinterpret_cast<xcb_configure_request_event_t*>(event));
					break;

				case XCB_CREATE_NOTIFY:
					if((handler = findHandler(reinterpret_cast<xcb_create_notify_event_t*>(event)->parent)) != NULL)
						handler->onCreateNotifyEvent(reinterpret_cast<xcb_create_notify_event_t*>(event));
					break;

				// Target is 'event'
				case XCB_BUTTON_PRESS:
					if((handler = findHandler(reinterpret_cast<xcb_button_press_event_t*>(event)->event)) != NULL)
						handler->onButtonPressEvent(reinterpret_cast<xcb_button_press_event_t*>(event));
					break;

				default:
					printf("WARNING: Unhandled event; type = %d\n", event->response_type);
					break;
			}

			xcb_flush(mConn);

			free(event);
		}
	}


	EventHandler*
	App::findHandler(
		xcb_window_t		window)
	{
		if(mWindowMap.count(window) == 0){
			printf("WARNING: Tried to find handler for unknown window; window = 0x%08X\n", window);
			return NULL;
		}

		return mWindowMap[window];
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

	uint32_t
	allocateColor(
		xcb_connection_t* 	conn,
		xcb_colormap_t 		cmap,
		uint16_t 			red,
		uint16_t 			green,
		uint16_t 			blue)
	{
		xcb_alloc_color_cookie_t	c = xcb_alloc_color(conn, cmap, red, green, blue);
		xcb_generic_error_t*		error;
		xcb_alloc_color_reply_t*	rsp = xcb_alloc_color_reply(conn, c, &error);
		if(error){
			printf("Failed to allocate color R:%04X G:%04X B:%04X. Error=%d\n", red, green, blue, error->error_code);
			exit(EXIT_FAILURE);
		}

		uint32_t			pixel = rsp->pixel;
		free(rsp);
		return pixel;
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
	xcb_render_picture_t		blackPen = createPen(conn, screen->root, formatARGB32, black);

	// Create fonts needed
	x::Font						systemFont(conn, formatA8, "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraBd.ttf", 12);

	// Everyone needs an atom cache
	x::AtomCache				atomCache(conn);

	uint32_t					colors[wm::kColorCount];
	colors[wm::kColorBlack] = screen->black_pixel;
	colors[wm::kColorWhite] = screen->white_pixel;
	colors[wm::kColorGray] = allocateColor(conn, screen->default_colormap, 0xCCCC, 0xCCCC, 0xCCCC);

	wm::App					app(conn, screen, systemFont, atomCache, formatA8, formatRGB24, formatARGB32, blackPen, colors);

	wm::RootWindow*		rootWindow = new wm::RootWindow(app, conn, screen, screen->root);

	app.run();

	xcb_disconnect(conn);

	return 0;
}
