/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2019 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include <cstdarg>
#include <type_traits>

#include "CommonTypes.hpp"

/*
============================================================================

					BYTE ORDER FUNCTIONS

============================================================================
*/

namespace
{

short ShortSwap(short l)
{
	byte b1 = l & 255;
	byte b2 = (l >> 8) & 255;

	return (b1 << 8) + b2;
};

short ShortNoSwap(short l)
{
	return l;
};

int LongSwap(int l)
{
	byte b1 = l & 255;
	byte b2 = (l >> 8) & 255;
	byte b3 = (l >> 16) & 255;
	byte b4 = (l >> 24) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
};

int LongNoSwap(int l)
{
	return l;
};

float FloatSwap(float f)
{
	union
	{
		float f;
		byte b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
};

float FloatNoSwap(float f)
{
	return f;
};

};

short BigShort(short l)
{
	if constexpr (std::endian::native == std::endian::little)
		return ShortSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return ShortNoSwap(l);
};

short LittleShort(short l)
{
	if constexpr (std::endian::native == std::endian::little)
		return ShortNoSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return ShortSwap(l);
};

int BigLong(int l)
{
	if constexpr (std::endian::native == std::endian::little)
		return LongSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return LongNoSwap(l);
};

int LittleLong(int l)
{
	if constexpr (std::endian::native == std::endian::little)
		return LongNoSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return LongSwap(l);
};

float BigFloat(float l)
{
	if constexpr (std::endian::native == std::endian::little)
		return FloatSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return FloatNoSwap(l);
};

float LittleFloat(float l)
{
	if constexpr (std::endian::native == std::endian::little)
		return FloatNoSwap(l);
	else if constexpr (std::endian::native == std::endian::big)
		return FloatSwap(l);
};

//============================================================================

/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
FIXME: make this buffer size safe someday
============
*/
char *va(const char *format, ...)
{
	va_list argptr;
	static char string[1024]{};

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
};