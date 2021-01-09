/*
Copyright (C) 2019-2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "CommonTypes.hpp"

using cvar_t = struct cvar_s;

interface IFile;

class CCvar
{
public:
	void Init();
	
	/// registers a cvar that already has the name, string, and optionally the
	/// archive elements set.
	void RegisterVariable(cvar_t *variable);

	/// equivelant to "<name> <variable>" typed at the console
	void Set(const char *var_name, const char *value);

	/// expands value to a string and calls Cvar_Set
	void SetValue(const char *var_name, float value);

	/// returns 0 if not defined or non numeric
	float VariableValue(const char *var_name) const;

	/// returns an empty string if not defined
	const char *VariableString(const char *var_name) const;

	/// attempts to match a partial variable name for command line completion
	/// returns nullptr if nothing fits
	const char *CompleteVariable(const char *partial) const;

	/// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
	/// command.  Returns true if the command was a variable reference that
	/// was handled. (print or change)
	bool HandleCommand();

	/// Writes lines containing "set variable value" for all variables
	/// with the archive flag set to true.
	void WriteVariables(IFile *f);

	cvar_t *FindVar(const char *var_name) const;
};