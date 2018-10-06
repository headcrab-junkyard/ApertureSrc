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
	MSG_WriteByte(mpBuf, nValue);
};

void CNetMsg::WriteChar(int nValue)
{
	MSG_WriteChar(mpBuf, nValue);
};

void CNetMsg::WriteShort(int nValue)
{
	MSG_WriteShort(mpBuf, nValue);
};

void CNetMsg::WriteLong(int nValue)
{
	MSG_WriteLong(mpBuf, nValue);
};

void CNetMsg::WriteAngle(float fValue)
{
	MSG_WriteAngle(mpBuf, fValue);
};

void CNetMsg::WriteCoord(float fValue)
{
	MSG_WriteCoord(mpBuf, fValue);
};

void CNetMsg::WriteString(const char *sValue)
{
	MSG_WriteString(mpBuf, sValue);
};

void CNetMsg::WriteEntity(int nValue)
{
	// TODO
	//MSG_WriteEntity(mpBuf, nValue);
};

int CNetMsg::ReadChar()
{
	return MSG_ReadChar(mpBuf);
};

int CNetMsg::ReadByte()
{
	return MSG_ReadByte(mpBuf);
};

int CNetMsg::ReadShort()
{
	return MSG_ReadShort(mpBuf);
};

int CNetMsg::ReadLong()
{
	return MSG_ReadLong(mpBuf);
};

float CNetMsg::ReadFloat()
{
	return MSG_ReadFloat(mpBuf);
};

float CNetMsg::ReadCoord()
{
	return MSG_ReadCoord(mpBuf);
};

float CNetMsg::ReadAngle()
{
	return MSG_ReadAngle(mpBuf);
};

char *CNetMsg::ReadString()
{
	return MSG_ReadString(mpBuf);
};