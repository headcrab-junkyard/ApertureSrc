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

#pragma once

#include "engine/INetMsg.hpp"

using sizebuf_t = struct sizebuf_s;

class CNetMsg final : public INetMsg
{
public:
	CNetMsg(sizebuf_t *apBuf);
	~CNetMsg();
	
	void WriteByte(int nValue) override;
	void WriteChar(int nValue) override;
	void WriteShort(int nValue) override;
	void WriteLong(int nValue) override;
	
	void WriteFloat(float fValue) override;
	void WriteAngle(float fValue) override;
	void WriteCoord(float fValue) override;
	
	void WriteString(const char *sValue) override;
	
	void WriteEntity(int nValue) override;
	
	//
	
	void BeginReading() override;
	
	int ReadChar() override;
	int ReadByte() override;
	int ReadShort() override;
	int ReadLong() override;
	
	float ReadFloat() override;
	float ReadCoord() override;
	float ReadAngle() override;
	
	char *ReadString() override;
private:
	sizebuf_t *mpBuf{nullptr};
};