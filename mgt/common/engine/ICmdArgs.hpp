/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2018, 2020 BlackPhrase
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
/// @brief console command arguments interface

#pragma once

#include "CommonTypes.hpp"

// The functions that execute commands get their parameters with this
// interface. The GetArgValue method will return a blank string
// if arg > argc, so string operations are always safe
interface ICmdArgs
{
	//virtual void Release() = 0;
	
	/**
	* Retrieves a specified token
	*
	* @param
	* @return an empty string if not present, not a nullptr
	*/
	virtual const char *GetByIndex(int anArgNum) const = 0;
	
	///
	virtual int CheckParm(const char *parm) const = 0;
	
	///
	virtual const char *GetArgValue(const char *asArg) const = 0;
	
	/// @return the number of tokens in the command string
	virtual int GetCount() const = 0;
	
	/// @return arguments set as string
	virtual const char *ToString() const = 0;
};