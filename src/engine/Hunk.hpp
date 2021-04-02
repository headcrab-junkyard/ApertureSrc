/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2021 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief hunk allocator

#pragma once

#include "CommonTypes.hpp"

/////////////////////////////////////////////////

interface ISystem;

class CHunk
{
public:
	CHunk(ISystem *apSystem, void *buf, int size);
	
	/// @return 0 filled memory
	void *Alloc(int size); 
	
	///
	void *AllocName(int size, const char *name);
	
	///
	void *HighAllocName(int size, const char *name);
	
	///
	void *TempAlloc(int size);
	
	///
	void FreeToHighMark(int mark);
	
	///
	void FreeToLowMark(int mark);
	
	///
	void Check();
	
	///
	int	GetHighMark();
	
	///
	int	GetLowMark() const;
private:
	ISystem *mpSystem{nullptr};
	
	byte *hunk_base{nullptr};
	int hunk_size{0};

	int hunk_low_used{0};
	int hunk_high_used{0};
};