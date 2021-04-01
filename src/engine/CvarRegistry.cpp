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

#include "quakedef.h"

#include "CvarRegistry.hpp"

#include "engine/ISystem.hpp"
#include "filesystem/IFile.hpp"

//EXPOSE_SINGLE_INTERFACE(CCvarRegistry, ICvarRegistry, OGS_CVARREGISTRY_INTERFACE_VERSION);

CCvarRegistry::CCvarRegistry(ISystem *apSystem) : mpSystem(apSystem){}

CCvarRegistry::~CCvarRegistry() = default;

void CCvarRegistry::Add(const char *asName, const char *asDefValue, int anFlags, const char *asDesc)
{
	Cvar_RegisterVariable(asName, asDefValue, anFlags); // TODO: description support
};

/*
============
Cvar_RegisterVariable

Adds a freestanding variable to the variable list.
============
*/
void CCvarRegistry::Register(cvar_t *variable)
{
	// first check to see if it has allready been defined
	if(Find(variable->name))
	{
		mpSystem->Printf("Can't register variable %s, allready defined\n", variable->name);
		return;
	};

	// check for overlap with a command
	if(Cmd_Exists(variable->name))
	{
		mpSystem->Printf("Cvar_RegisterVariable: %s is a command\n", variable->name);
		return;
	};

	// copy the value off, because future sets will Z_Free it
	char *oldstr = variable->string;
	variable->string = (char*)Z_Malloc(Q_strlen(variable->string) + 1);
	Q_strcpy(variable->string, oldstr);
	variable->value = Q_atof(variable->string);

	// link the variable in
	variable->next = cvar_vars;
	cvar_vars = variable;
}

/*
============
Cvar_FindVar
============
*/
cvar_t *CCvarRegistry::Find(const char *var_name) const
{
	for(auto var = cvar_vars; var; var = var->next)
		if(!Q_strcmp(var_name, var->name))
			return var;

	return nullptr;
};

/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void CCvarRegistry::WriteToFile(IFile *apFile)
{
	for(auto var = cvar_vars; var; var = var->next)
		if(var->flags & FCVAR_ARCHIVE)
			apFile->Printf("%s \"%s\"\n", var->name, var->string);
};

/*
============
Cvar_CompleteVariable
============
*/
const char *CCvarRegistry::Complete(const char *partial) const
{
	int len{Q_strlen(partial)};

	if(!len)
		return nullptr;

	// check functions
	for(auto cvar = cvar_vars; cvar; cvar = cvar->next)
		if(!Q_strncmp(partial, cvar->name, len))
			return cvar->name;

	return nullptr;
};

bool CCvar::HandleCommand()
{
	return Cvar_Command();
};