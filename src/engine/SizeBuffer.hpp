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

using sizebuf_t = struct sizebuf_s;

class CSizeBuffer
{
public:
	CSizeBuffer(int startsize);
	~CSizeBuffer();
	//void Alloc(sizebuf_t *buf, int startsize);
	//void Free(sizebuf_t *buf);
	
	void Clear();
	
	void Write(const void *data, int length);
	void Print(const char *data); // strcats onto the sizebuf
	
	void *GetSpace(int length);
private:
	sizebuf_t *buf;
};