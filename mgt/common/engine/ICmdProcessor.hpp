/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2020 BlackPhrase
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
/// @brief command processor interface

#pragma once

#include "CommonTypes.hpp"
#include "tier1/interface.h"

interface ICmdProcessor
{
	/// Insert command mode
	enum class InsertMode : int
	{
		Front, // Insert
		Back // Append
	};
	
	///
	virtual void QueCmd(const char *asText, InsertMode aeMode = InsertMode::Back) = 0; // TODO: leave only back?
	
	///
	virtual void ExecCmd(const char *asText) = 0;
	
	///
	virtual void ExecQue() = 0;
};