#ifndef MANIX_COMMON_ATOMCACHE_H
#define MANIX_COMMON_ATOMCACHE_H

#include <map>
#include <string>
#include <xcb/xcb.h>

namespace manix {
namespace x {
	
	class AtomCache {
	public:
						AtomCache(
							xcb_connection_t*		conn);
			
		xcb_atom_t	get(
							const std::string&	name);
							
	private:
		xcb_connection_t*			mConn;
		std::map<std::string, xcb_atom_t>	mAtoms;	
	};
	
} // x
} // manix

#endif // MANIX_COMMON_AUTOCACHE_H