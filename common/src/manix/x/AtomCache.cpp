#include <cstdlib>
#include <xcb/xcb.h>

#include "manix/x/AtomCache.h"

namespace manix {
namespace x {
	
	AtomCache::AtomCache(
		xcb_connection_t*		conn)
		: mConn(conn)
	{
		
	}
	
	xcb_atom_t
	AtomCache::get(
		const std::string&	name)
	{
		if(mAtoms.count(name) == 0){
			xcb_intern_atom_cookie_t	cookie = xcb_intern_atom(mConn, false, name.length(), name.c_str());
			
			xcb_generic_error_t*			error;
			xcb_intern_atom_reply_t*	reply = xcb_intern_atom_reply(mConn, cookie, &error);
			printf("Interned atom; errorcode = %d\n", error->error_code);
			
			mAtoms[name] = reply->atom;
			
			free(reply);
		}
		
		return mAtoms[name];
	}
	
} // x
} // manix