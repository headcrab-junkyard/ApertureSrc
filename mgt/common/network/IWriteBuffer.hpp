/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2019 BlackPhrase
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
/// @brief writing buffer interface

#pragma once

#include "CommonTypes.hpp"

interface IWriteBuffer
{
	///
	virtual void WriteByte(int anValue) = 0;
	
	///
	virtual void WriteChar(int anValue) = 0;
	
	///
	virtual void WriteShort(int anValue) = 0;
	
	///
	virtual void WriteLong(int anValue) = 0;
	
	///
	virtual void WriteFloat(float afValue) = 0;
	
	///
	virtual void WriteAngle(float afValue) = 0;
	
	///
	virtual void WriteCoord(float afValue) = 0;
	
	///
	virtual void WriteString(const char *asValue) = 0;
	
	///
	virtual void WriteEntity(int anValue) = 0;
};