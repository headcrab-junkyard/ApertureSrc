/*
 * This file is part of Magenta Engine
 *
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

#pragma once

#include "VGUI.hpp"
#include "Color.h"

class KeyValues;

namespace vgui3
{

struct IScheme;

struct IBorder
{
	enum class Side : int
	{
		Left = 0,
		Top,
		Right,
		Bottom
	};

	///
	virtual void Paint(VPANEL panel) = 0;
	
	///
	virtual void Paint(int x0, int y0, int x1, int y1) = 0;
	
	///
	virtual void Paint(int x0, int y0, int x1, int y1, int breakSide, int breakStart, int breakStop) = 0;
	
	///
	virtual void SetInset(int left, int top, int right, int bottom) = 0;
	
	///
	virtual void GetInset(int &left, int &top, int &right, int &bottom) = 0;
	
	///
	virtual void AddLine(Side aeSide, Color color, int anStartOffset, int anEndOffset) = 0;
	
	///
	virtual void ApplySchemeSettings(IScheme *apScheme, KeyValues *inResourceData) = 0;
	
	///
	virtual void ParseSideSettings(int anSideIndex, KeyValues *inResourceData, IScheme *apScheme) = 0;
	
	///
	virtual const char *GetName() = 0;
	
	///
	virtual void SetName(const char *asName) = 0;
};

}; // namespace vgui3