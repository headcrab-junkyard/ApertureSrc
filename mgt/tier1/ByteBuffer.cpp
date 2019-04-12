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

#include "CommonTypes.hpp"
#include "tier1/ByteBuffer.hpp"

#ifdef MGT_PARANOID
#	include "engine/ISystem.hpp"
#endif

CByteBuffer::CByteBuffer(sizebuf_t *apData) : mpData(apData)
{
	BeginReading();
};

#ifdef MGT_PARANOID
	CByteBuffer::CByteBuffer(sizebuf_t *apData, ISystem *apSystem = nullptr) : CByteBuffer(apData), mpSystem(apSystem){}
#endif

CByteBuffer::~CByteBuffer() = default;

// Writing methods

void CByteBuffer::WriteByte(int nValue)
{
#ifdef MGT_PARANOID
	if(nValue < 0 || nValue > 255)
		mpSystem->Error("MSG_WriteByte: range error");
#endif

	auto buf{(byte*)mpData->GetSpace(1)};
	buf[0] = nValue;
};

void CByteBuffer::WriteChar(int nValue)
{
#ifdef MGT_PARANOID
	if(nValue < -128 || nValue > 127)
		mpSystem->Error("MSG_WriteChar: range error");
#endif

	auto buf{(byte*)mpData->GetSpace(1)};
	buf[0] = nValue;
};

void CByteBuffer::WriteShort(int nValue)
{
#ifdef MGT_PARANOID
	if(nValue < ((short)0x8000) || nValue > (short)0x7fff)
		mpSystem->Error("MSG_WriteShort: range error");
#endif

	auto buf{(byte*)mpData->GetSpace(2)};
	buf[0] = nValue & 0xff;
	buf[1] = nValue >> 8;
};

void CByteBuffer::WriteLong(int nValue)
{
	auto buf{(byte*)mpData->GetSpace(4)};
	buf[0] = nValue & 0xff;
	buf[1] = (nValue >> 8) & 0xff;
	buf[2] = (nValue >> 16) & 0xff;
	buf[3] = nValue >> 24;
};

void CByteBuffer::WriteFloat(float fValue)
{
	union
	{
		float f;
		int l;
	} dat;

	dat.f = fValue;
	dat.l = LittleLong(dat.l);

	mpData->Write(&dat.l, 4);
};

void CByteBuffer::WriteAngle(float fValue)
{
	WriteByte(((int)fValue * 256 / 360) & 255);
};

void CByteBuffer::WriteCoord(float fValue)
{
	WriteShort((int)(fValue * 8));
};

void CByteBuffer::WriteString(const char *sValue)
{
	if(!sValue)
		mpData->Write("", 1);
	else
		mpData->Write(sValue, Q_strlen(sValue) + 1);
};

void CByteBuffer::WriteEntity(int nValue)
{
	// TODO
	//MSG_WriteEntity(nValue);
};

// Reading methods

int CByteBuffer::ReadByte()
{
	if(msg_readcount + 1 > mpData->cursize)
	{
		msg_badread = true;
		return -1;
	};

	int c{(byte)mpData->data[msg_readcount]};
	msg_readcount++;

	return c;
};

int CByteBuffer::ReadChar()
{
	if(msg_readcount + 1 > mpData->cursize)
	{
		msg_badread = true;
		return -1;
	};

	int c{(signed char)mpData->data[msg_readcount]};
	msg_readcount++;

	return c;
};

int CByteBuffer::ReadShort()
{
	if(msg_readcount + 2 > mpData->cursize)
	{
		msg_badread = true;
		return -1;
	};

	int c{(short)(mpData->data[msg_readcount] + (mpData->data[msg_readcount + 1] << 8))};

	msg_readcount += 2;

	return c;
};

int CByteBuffer::ReadLong()
{
	if(msg_readcount + 4 > mpData->cursize)
	{
		msg_badread = true;
		return -1;
	};

	int c{mpData->data[msg_readcount] + (mpData->data[msg_readcount + 1] << 8) + (mpData->data[msg_readcount + 2] << 16) + (mpData->data[msg_readcount + 3] << 24)};

	msg_readcount += 4;

	return c;
};

float CByteBuffer::ReadFloat()
{
	union
	{
		byte b[4];
		float f;
		int l;
	} dat;

	dat.b[0] = mpData->data[msg_readcount];
	dat.b[1] = mpData->data[msg_readcount + 1];
	dat.b[2] = mpData->data[msg_readcount + 2];
	dat.b[3] = mpData->data[msg_readcount + 3];
	msg_readcount += 4;

	dat.l = LittleLong(dat.l);

	return dat.f;
};

float CByteBuffer::ReadCoord()
{
	return ReadShort() * (1.0 / 8);
};

float CByteBuffer::ReadAngle()
{
	return ReadChar() * (360.0 / 256);
};

char *CByteBuffer::ReadString()
{
	static char string[2048];
	int l, c;

	l = 0;
	do
	{
		c = ReadChar(mpData);
		if(c == -1 || c == 0)
			break;
		string[l] = c;
		l++;
	}
	while(l < sizeof(string) - 1);

	string[l] = 0;
	return string;
};

void CByteBuffer::BeginReading()
{
	msg_readcount = 0;
	msg_badread = false;
};