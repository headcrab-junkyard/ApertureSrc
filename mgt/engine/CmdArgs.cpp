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

#include "quakedef.h"
#include "CmdArgs.hpp"

CCmdArgs::CCmdArgs(const char *text)
{
	Cmd_TokenizeString(text);
};

CCmdArgs::~CCmdArgs() = default;

const char *CCmdArgs::GetByIndex(int anArg) const
{
	return Cmd_Argv(anArg);
};

int CCmdArgs::CheckParm(const char *parm) const
{
	return Cmd_CheckParm(parm);
};

const char *CCmdArgs::GetArgValue(const char *asArg) const
{
	auto nArg{Cmd_CheckParm(asArg)};
	return nArg? Cmd_Argv(nArg) : "";
};

int CCmdArgs::GetCount() const
{
	return Cmd_Argc();
};

const char *CCmdArgs::ToString() const
{
	return Cmd_Args();
};