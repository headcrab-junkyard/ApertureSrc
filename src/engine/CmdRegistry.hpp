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

#pragma once

#include "CommonTypes.hpp"

//#include "engine/ICmdRegistry.hpp"

interface ISystem;
interface ICmdArgs;

using xcommand_t = void (*)(const ICmdArgs &aArgs);

class CCmd
{
public:
	static void Init();
};

/*
class CCmdRegistry
{
public:
	// called by the init functions of other parts of the program to
	// register commands and functions to call for them.
	// The cmd_name is referenced later, so it should not be in temp memory
	//
	// Q3 rules:
	//
	// if function is NULL, the command will be forwarded to the server
	// as a clc_clientCommand instead of executed locally
	void Add(const char *asName, xcommand_t afnCallback) override;
};
*/

class CCmdRegistry //final : public ICmdRegistry
{
public:
	CCmdRegistry(ISystem *apSystem);
	~CCmdRegistry();
	
	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	void Add(const char *cmd_name, xcommand_t function);

	void Remove(const char *cmd_name);
	
	/// used by the cvar code to check for cvar / command name overlap
	bool Exists(const char *cmd_name) const;

	/// attempts to match a partial command for automatic command line completion
	/// returns nullptr if nothing fits
	const char *Complete(const char *partial) const;

	/// adds the current command line as a clc_stringcmd to the client message.
	/// things like godmode, noclip, etc, are commands directed to the server,
	/// so when they are typed in at the console, they will need to be forwarded
	//void ForwardToServer(const ICmdArgs &aArgs); // TODO

	/// used by command functions to send output to either the graphics console or
	/// passed as a print message to the client
	//void Print(const char *text); // TODO
private:
	ISystem *mpSystem{nullptr};
	
	cmd_function_t *cmd_functions{nullptr}; ///< possible commands to execute
};