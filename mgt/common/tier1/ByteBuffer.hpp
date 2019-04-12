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
/// @brief (network) message I/O functions - handles byte ordering and avoids alignment errors

#pragma once

#include "qcommon.h"

using sizebuf_t = struct sizebuf_s;

//extern struct usercmd_s nullcmd; // TODO: qw

struct ISystem;

class CByteBuffer
{
public:
	CByteBuffer(sizebuf_t *apData);
#ifdef MGT_PARANOID
	CByteBuffer(sizebuf_t *apData, ISystem *apSystem = nullptr);
#endif
	~CByteBuffer();
	
	// Writing methods
	
	///
	void WriteByte(int nValue);

	///
	void WriteChar(int nValue);
	
	///
	void WriteShort(int nValue);

	///
	void WriteLong(int nValue);
	
	///
	void WriteFloat(float fValue);
	
	///
	void WriteAngle(float fValue);
	
	///
	void WriteCoord(float fValue);
	
	///
	void WriteString(const char *sValue);
	
	///
	void WriteEntity(int nValue);
	
	// Reading methods

	///
	int ReadByte();
	
	/// returns -1 and sets msg_badread if no more characters are available
	int ReadChar();

	///
	int ReadShort();

	///
	int ReadLong();
	
	///
	float ReadFloat();
	
	///
	float ReadCoord();
	
	///
	float ReadAngle();
	
	///
	char *ReadString();
	
	///
	int GetReadCount() const {return msg_readcount;} // TODO: used by ServerUser
private:
	///
	void BeginReading(); // TODO: Reset?
	
	sizebuf_t *mpData{nullptr};

#ifdef MGT_PARANOID
	ISystem *mpSystem{nullptr};
#endif

	int msg_readcount{0};
	bool msg_badread{false}; ///< set if a read goes beyond end of message
};