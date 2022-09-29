/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2019, (2020?-)2021 BlackPhrase
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

#pragma once

//#include "CommonTypes.hpp"

#include "engine/ICmdArgs.hpp"

class CSystem;

class CCmdArgs /*final*/ : public ICmdArgs
{
public:
	CCmdArgs(CSystem *apSystem, const char *asCmdLine);
	~CCmdArgs();
	
	/// @return number of arguments provided
	int GetCount() const override;
	
	/// @return an empty string, not a nullptr if arg > argc, so string operations are always safe
	const char *GetByIndex(int anIndex) const override;
	
	//const char *GetByName(const char *asArg) const override;
	
	/// Returns the position (1 to argc-1) in the command's argument list
	/// where the given parameter apears, or 0 if not present
	int CheckParm(const char *parm) const override;
	
	/// @return a string containing all the arguments
	const char *ToString() const override;
private:
	/// Takes a null terminated string.
	/// Does not need to be /n terminated.
	/// Breaks the string up into arg tokens
	void TokenizeString(const char *asText);
private:
	static constexpr auto MAX_ARGS{80};
	
	char *cmd_argv[MAX_ARGS]{};
	
	CSystem *mpSystem{nullptr};
	
	const char *cmd_args{nullptr};
	
	int cmd_argc{0};
};