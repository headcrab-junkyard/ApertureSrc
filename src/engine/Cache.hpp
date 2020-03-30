/*
Copyright (C) 2019-2020 BlackPhrase

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

class CCache
{
public:
	/// Returns nullptr if all purgable data was tossed and there still
	/// wasn't enough room
	void *Alloc(cache_user_t *c, int size, const char *name);

	void Free(cache_user_t *c);

	void *Check(cache_user_t *c);
	/// returns the cached data, and moves to the head of the LRU list
	/// if present, otherwise returns nullptr

	void Report();
	void Flush();
};