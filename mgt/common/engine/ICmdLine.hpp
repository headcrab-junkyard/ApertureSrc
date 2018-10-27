/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief command line interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_CMDLINE_INTERFACE_VERSION{"MGTCmdLine001Alpha"};

/*
interface ICommandLine
{
public:
	virtual void SetArg(const char *asArg, const char *asValues) = 0;
	virtual void SetArg(const char *asArg, int anValue) = 0;
	virtual void AddArg(const char *asArg, const char *asValues) = 0;
	virtual bool HasArg(const char *asArg) const = 0;
	virtual void RemoveArg(const char *asArg) = 0;
	
	virtual const char *GetArgValue(const char *asArg, const char *asDefVal) const = 0;
	virtual int GetArgValue(const char *asArg, int anDefVal) const = 0;
	virtual float GetArgValue(const char *asArg, float afDefVal) const = 0;
	
	virtual const char *CheckArg(const char *asArg, const char **asValues = 0) const = 0;
	virtual const char *GetArg(int anID) const = 0;
};

class ICommandLine
{
public:
	virtual const char *CheckParm(const char *psz, const char **ppszValue = 0) const = 0;
	
	virtual void AppendParm(const char *pszParm, const char *pszValues) = 0;
	virtual void SetParm(const char *pszParm, const char *pszValues) = 0;
	virtual void SetParm(const char *pszParm, int iValue) = 0;
};

*/

// minimal
interface ICmdLine : public IBaseInterface
{
	virtual void ParseString(const char *asArgs) = 0;
	virtual void ParseArgs(int argc, char **argv) = 0;
	
	virtual const char *GetArgValue(const char *asName) const = 0;
	
	virtual void RemoveArg(const char *asName) = 0;
	virtual bool HasArg(const char *asName) const = 0;
	
	virtual void ToString(char *asOutStr, int anStrSize) const = 0;
	
	virtual int GetArgCount() const = 0;
};