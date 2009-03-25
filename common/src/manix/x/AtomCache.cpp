/*
 * Copyright (c) 2009 Magnus Sjöstrand <magnus@manix.nu>
 *
 * This file is part of manix.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
