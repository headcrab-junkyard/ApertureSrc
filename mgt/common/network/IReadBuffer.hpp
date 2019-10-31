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
/// @brief reading buffer interface

#pragma once

#include "CommonTypes.hpp"

interface IReadBuffer
{
	///
	virtual int ReadByte() = 0;
	
	///
	virtual int ReadChar() = 0;
	
	///
	virtual int ReadShort() = 0;
	
	///
	virtual int ReadLong() = 0;
	
	///
	virtual float ReadFloat() = 0;
	
	///
	virtual float ReadCoord() = 0;
	
	///
	virtual float ReadAngle() = 0;
	
	///
	virtual const char *ReadString() = 0;
};