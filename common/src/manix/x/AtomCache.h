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
