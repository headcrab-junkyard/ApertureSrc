/*
Copyright (C) 2019-2021 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "CommonTypes.hpp"

interface ISystem;

using cache_system_t = struct cache_system_s;

class CCache
{
public:
	CCache(ISystem *apSystem);
	
	/// Returns nullptr if all purgable data was tossed and there still
	/// wasn't enough room
	void *Alloc(cache_user_t *c, int size, const char *name);

	void Free(cache_user_t *c);

	void *Check(cache_user_t *c);
	/// returns the cached data, and moves to the head of the LRU list
	/// if present, otherwise returns nullptr

	void Report();
	void Flush();
private:
	cache_system_t *TryAlloc(int size, bool nobottom);
	
	void MakeLRU(cache_system_t *cs);
	void UnlinkLRU(cache_system_t *cs);
private:
	ISystem *mpSystem{nullptr};
};