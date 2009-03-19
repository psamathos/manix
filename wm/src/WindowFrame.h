#ifndef WM_WINDOWFRAME_H
#define WM_WINDOWFRAME_H

#include <string>
#include <xcb/xcb.h>

namespace manix {
namespace wm {

	class App;
	class ClientWindow;

	class WindowFrame {
	public:
							WindowFrame(
								App&					app,
								xcb_connection_t*		conn,
								ClientWindow&			client);

		virtual void		show() = 0;

		virtual void		moveClientTo(
									int32_t&				x,
									int32_t&				y) = 0;

		virtual void		resizeClientTo(
									uint32_t&				width,
									uint32_t&				height) = 0;

		virtual void		adjustClientToFrame() = 0;
		virtual void		adjustFrameToClient() = 0;

		void				setTitle(
								const std::wstring&			title);
		void				getTitle(
								std::wstring&				title) { title = mTitle; }

		ClientWindow&		getClientWindow() { return mClientWindow; }

	protected:

		App&				getApp() { return mApp; }

	private:
		xcb_connection_t*	mConn;
		App&				mApp;
		ClientWindow&		mClientWindow;
		std::wstring		mTitle;

		virtual void		onTitleChanged(
								const std::wstring&			newTitle);
	};

} // wm
} // manix

#endif // WM_WINDOWFRAME_H
