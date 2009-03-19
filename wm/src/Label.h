#ifndef WM_LABEL_H
#define WM_LABEL_H

#include <string>
#include <xcb/xcb.h>
#include <xcb/render.h>

#include "manix/x/Font.h"
#include "manix/x/Window.h"

#include "EventHandler.h"

namespace manix {
namespace wm {

	class App;

	class Label : private EventHandler {
	public:

		typedef enum {
			kHAlignLeft,
			kHAlignCenter,
			kHAligtRight
		}HAlign;

								Label(
									App&						app,
									xcb_screen_t*				screen,
									xcb_window_t				parent,
									xcb_render_picture_t		color,
									x::Font*					font,
									HAlign						align,
									uint32_t					background,
									int32_t						x,
									int32_t						y,
									uint32_t					width,
									uint32_t					height,
									const std::wstring&			text);

		void					setText(
									const std::wstring&			text);
		uint32_t				getTextWidth() const { return mTextWidth; }

		void					show();

		void					moveTo(
									int32_t						x,
									int32_t						y) { mWindow.moveTo(x,y); }
		void					resizeTo(
									uint32_t					width,
									uint32_t					height) { mWindow.resizeTo(width, height); }

	private:
		App&					mApp;
		x::Window				mWindow;
		std::wstring			mText;
		x::Font*				mFont;
		xcb_render_picture_t	mWindowPict;
		HAlign					mHAlign;
		uint16_t				mTextWidth;
		uint16_t				mWidth;
		uint16_t				mHeight;

		void					onEvent(
									xcb_expose_event_t*	event);

		void					updateText();

		void					redraw();

	};

} // wm
} // manix

#endif // WM_LABEL_H
