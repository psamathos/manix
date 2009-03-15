#include "WindowFrame.h"

namespace manix {
namespace wm {

	WindowFrame::WindowFrame(
		App&					app,
		xcb_connection_t*		conn,
		ClientWindow&			client)
		: mConn(conn)
		, mApp(app)
		, mClientWindow(client)
	{

	}

	void
	WindowFrame::setTitle(
		const std::wstring&		title)
	{
		if(title != mTitle){
			mTitle = title;
			onTitleChanged(mTitle);
		}
	}

	void
	WindowFrame::onTitleChanged(
		const std::wstring&		newTitle)
	{

	}

} // wm
} // manix
