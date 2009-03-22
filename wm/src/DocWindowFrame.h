#ifndef WM_DOCWINDOWFRAME_H
#define WM_DOCWINDOWFRAME_H

#include "manix/x/Window.h"

#include "WindowFrame.h"

namespace manix {
namespace wm {

	class Label;

	class DocWindowFrame : public WindowFrame, private EventHandler {
	public:
						DocWindowFrame(
							App&					app,
							xcb_connection_t*		conn,
							xcb_window_t			parent,
							xcb_screen_t*			screen,
							ClientWindow&			client);

		void			show();

		void			moveClientTo(
							int32_t&				x,
							int32_t&				y);

		void			resizeClientTo(
							uint32_t&				width,
							uint32_t&				height);

		void			adjustClientToFrame();
		void			adjustFrameToClient();

	private:
		xcb_gcontext_t	mGc;
		x::Window		mFrameWindow;
		Label*			mWindowLabel;

		uint32_t		calcWidthFromClient();
		uint32_t		calcHeightFromClient();

		void			onTitleChanged(
							const std::wstring&		title);

		void			onButtonPressEvent(
							xcb_button_press_event_t*	event);
		void			onButtonReleaseEvent(
							xcb_button_release_event_t*	event);
		void			onExposeEvent(
							xcb_expose_event_t*		event);
	};

} // wm
} // manix

#endif // WM_DOCWINDOWFRAME_H
