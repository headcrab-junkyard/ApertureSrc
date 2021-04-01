/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, (2020?) 2021 BlackPhrase
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
#include "CmdRegistry.hpp"

void CCmd::Init()
{
	Cmd_Init();
};

//EXPOSE_SINGLE_INTERFACE(CCmdRegistry, ICmdRegistry, OGS_CMDREGISTRY_INTERFACE_VERSION);

CCmdRegistry::CCmdRegistry(ISystem *apSystem) : mpSystem(apSystem){}

CCmdRegistry::~CCmdRegistry() = default;

/*
============
Cmd_AddCommand
============
*/
void CCmdRegistry::Add(const char *cmd_name, xcommand_t function)
{
	if(host_initialized) // because hunk allocation would get stomped
		mpSystem->Error("Cmd_AddCommand after host_initialized");

	// fail if the command is a variable name
	if(Cvar_VariableString(cmd_name)[0])
	{
		mpSystem->Printf("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		return;
	};

	cmd_function_t *cmd{nullptr};

	// fail if the command already exists
	for(cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if(!Q_strcmp(cmd_name, cmd->name))
		{
			mpSystem->Printf("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		};
	};

	cmd = (cmd_function_t *)Hunk_Alloc(sizeof(cmd_function_t));
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
};

/*
============
Cmd_Exists
============
*/
bool CCmdRegistry::Exists(const char *cmd_name) const
{
	for(auto cmd = cmd_functions; cmd; cmd = cmd->next)
		if(!Q_strcmp(cmd_name, cmd->name))
			return true;

	return false;
};

const char *CCmdRegistry::Complete(const char *partial) const
{
	return Cmd_CompleteCommand(partial);
};

/*
void CCmdRegistry::ForwardToServer(const ICmdArgs &aArgs)
{
	Cmd_ForwardToServer();
};

void CCmdRegistry::Print(const char *text)
{
	Cmd_Print(text);
};
*/