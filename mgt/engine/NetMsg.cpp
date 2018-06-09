/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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

#include "quakedef.h"
#include "NetMsg.hpp"

CNetMsg::CNetMsg(sizebuf_t *apBuf) : mpBuf(apBuf){}

CNetMsg::~CNetMsg() = default;

void CNetMsg::WriteByte(int nValue)
{
	SZ_WriteByte(mpBuf, nValue);
};

void CNetMsg::WriteChar(int nValue)
{
	SZ_WriteChar(mpBuf, nValue);
};

void CNetMsg::WriteShort(int nValue)
{
	SZ_WriteShort(mpBuf, nValue);
};

void CNetMsg::WriteLong(int nValue)
{
	SZ_WriteLong(mpBuf, nValue);
};

void CNetMsg::WriteAngle(float fValue)
{
	SZ_WriteAngle(mpBuf, fValue);
};

void CNetMsg::WriteCoord(float fValue)
{
	SZ_WriteCoord(mpBuf, fValue);
};

void CNetMsg::WriteString(const char *sValue)
{
	SZ_WriteString(mpBuf, sValue);
};

void CNetMsg::WriteEntity(int nValue)
{
	// TODO
	//SZ_WriteEntity(mpBuf, nValue);
};