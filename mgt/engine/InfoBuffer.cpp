/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "InfoBuffer.hpp"

CInfoBuffer::CInfoBuffer(char *asBuf) : buf(asBuf){}

CInfoBuffer::~CInfoBuffer() = default;

void CInfoBuffer::SetKeyValue(const char *asKey, const char *asValue)
{
	Info_SetValueForKey(buf, asKey, asValue, MAX_INFO_STRING); // TODO: maxsize = ???
};

const char *CInfoBuffer::GetKeyValue(const char *asKey) const
{
	return Info_ValueForKey(buf, asKey);
};

void CInfoBuffer::RemoveKey(const char *asKey)
{
	Info_RemoveKey(buf, asKey);
};