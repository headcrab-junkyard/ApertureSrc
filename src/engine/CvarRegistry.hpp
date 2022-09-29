/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2019, 2021 BlackPhrase
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

#include "engine/ICvarRegistry.hpp"

interface ISystem;
interface ICmdArgs;

class CCvarRegistry final : public ICvarRegistry
{
public:
	CCvarRegistry(ISystem *apSystem);
	~CCvarRegistry();
	
	void Add(const char *asName, const char *asDefValue, int anFlags, const char *asDesc) override;
	
	/// registers a cvar that already has the name, string, and optionally the
	/// archive elements set.
	void Register(cvar_t *variable) override;
	
	cvar_t *Find(const char *var_name) const;
	
	/// Writes lines containing "set variable value" for all variables
	/// with the archive flag set to true.
	void WriteToFile(IFile *apFile) override;
	
	/// attempts to match a partial variable name for command line completion
	/// returns nullptr if nothing fits
	const char *Complete(const char *partial) const override;
	
	/// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
	/// command.  Returns true if the command was a variable reference that
	/// was handled. (print or change)
	bool HandleCommand(const ICmdArgs &aArgs);
private:
	ISystem *mpSystem{nullptr};
};