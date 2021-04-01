/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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
/// @brief script command processing module

#include "quakedef.h"
#include "CmdArgs.hpp"

#include "engine/cdll_int.h"

cmdalias_t *cmd_alias{nullptr};

bool cmd_wait{false};

//=============================================================================

/*
============
Cmd_Wait_f

Causes execution of the remainder of the command buffer to be delayed until
next frame.  This allows commands like:
bind g "impulse 5 ; +attack ; wait ; -attack ; impulse 2"
============
*/
void Cmd_Wait_f(const ICmdArgs &apArgs)
{
	cmd_wait = true;
};

/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/

/*
===============
Cmd_StuffCmds_f

Adds command line parameters as script statements
Commands lead with a +, and continue until a - or another +
quake +prog jctest.qp +cmd amlev1
quake -nosound +cmd amlev1
===============
*/
void Cmd_StuffCmds_f(const ICmdArgs &apArgs)
{
	int i, j;
	int s;
	char *text, *build, c;

	if(apArgs.GetCount() != 1)
	{
		gpSystem->Printf("stuffcmds : execute command line parameters\n");
		return;
	};

	// build the combined string to parse from
	s = 0;
	for(i = 1; i < com_argc; i++)
	{
		if(!com_argv[i])
			continue; // NEXTSTEP nulls out -NXHost
		s += Q_strlen(com_argv[i]) + 1;
	};

	if(!s)
		return;

	text = (char *)Z_Malloc(s + 1);
	text[0] = 0;
	for(i = 1; i < com_argc; i++)
	{
		if(!com_argv[i])
			continue; // NEXTSTEP nulls out -NXHost
		Q_strcat(text, com_argv[i]);
		if(i != com_argc - 1)
			Q_strcat(text, " ");
	};

	// pull out the commands
	build = (char *)Z_Malloc(s + 1);
	build[0] = 0;

	for(i = 0; i < s - 1; i++)
	{
		if(text[i] == '+')
		{
			i++;

			for(j = i; (text[j] != '+') && (text[j] != '-') && (text[j] != 0); j++)
				;

			c = text[j];
			text[j] = 0;

			Q_strcat(build, text + i);
			Q_strcat(build, "\n");
			text[j] = c;
			i = j - 1;
		};
	};

	if(build[0])
		Cbuf_InsertText(build);

	Z_Free(text);
	Z_Free(build);
};

/*
===============
Cmd_Exec_f
===============
*/
void Cmd_Exec_f(const ICmdArgs &apArgs)
{
	char *f;
	int mark;

	if(apArgs.GetCount() != 2)
	{
		gpSystem->Printf("exec <filename> : execute a script file\n");
		return;
	};

	mark = Hunk_LowMark();
	f = (char *)COM_LoadHunkFile(apArgs.GetByIndex(1));
	if(!f)
	{
		gpSystem->Printf("couldn't exec %s\n", apArgs.GetByIndex(1));
		return;
	};
	gpSystem->Printf("execing %s\n", apArgs.GetByIndex(1));

	Cbuf_InsertText(f);
	Hunk_FreeToLowMark(mark);
};

/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
void Cmd_Echo_f(const ICmdArgs &apArgs)
{
	for(int i = 1; i < apArgs.GetCount(); ++i)
		gpSystem->Printf("%s ", apArgs.GetByIndex(i));
	gpSystem->Printf("\n");
};

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/

char *CopyString(const char *in)
{
	char *out = (char *)Z_Malloc(Q_strlen(in) + 1);
	Q_strcpy(out, in);
	return out;
};

void Cmd_Alias_f(const ICmdArgs &apArgs)
{
	cmdalias_t *a;
	char cmd[1024];
	int i, c;
	const char *s;

	if(apArgs.GetCount() == 1)
	{
		gpSystem->Printf("Current alias commands:\n");
		for(a = cmd_alias; a; a = a->next)
			gpSystem->Printf("%s : %s\n", a->name, a->value);
		return;
	};

	s = Cmd_Argv(1);
	if(Q_strlen(s) >= MAX_ALIAS_NAME)
	{
		gpSystem->Printf("Alias name is too long\n");
		return;
	};

	// if the alias allready exists, reuse it
	for(a = cmd_alias; a; a = a->next)
	{
		if(!strcmp(s, a->name))
		{
			Z_Free(a->value);
			break;
		};
	};

	if(!a)
	{
		a = (cmdalias_t *)Z_Malloc(sizeof(cmdalias_t));
		a->next = cmd_alias;
		cmd_alias = a;
	};
	strcpy(a->name, s);

	// copy the rest of the command line
	cmd[0] = 0; // start out with a null string
	c = apArgs.GetCount();
	for(i = 2; i < c; i++)
	{
		strcat(cmd, apArgs.GetByIndex(i));
		if(i != c)
			strcat(cmd, " ");
	};
	strcat(cmd, "\n");

	a->value = CopyString(cmd);
};

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

typedef struct cmd_function_s
{
	struct cmd_function_s *next;
	const char *name;
	xcommand_t function;
} cmd_function_t;



cmd_source_t cmd_source;

static cmd_function_t *cmd_functions; // possible commands to execute

/*
============
Cmd_Init
============
*/
void Cmd_Init()
{
	//
	// register our commands
	//
	Cmd_AddCommand("stuffcmds", Cmd_StuffCmds_f);
	Cmd_AddCommand("exec", Cmd_Exec_f);
	Cmd_AddCommand("echo", Cmd_Echo_f);
	Cmd_AddCommand("alias", Cmd_Alias_f);
	Cmd_AddCommand("wait", Cmd_Wait_f);
};

/*
============
Cmd_CompleteCommand
============
*/
const char *Cmd_CompleteCommand(const char *partial)
{
	cmd_function_t *cmd;

	int len = Q_strlen(partial);

	if(!len)
		return nullptr;

	// check functions
	for(cmd = cmd_functions; cmd; cmd = cmd->next)
		if(!Q_strncmp(partial, cmd->name, len))
			return cmd->name;

	return nullptr;
}
/*
============
Cmd_List_f
============
*/
void Cmd_List_f()
{
	cmd_function_t	*cmd;
	int				i;

	i = 0;
	for (cmd=cmd_functions ; cmd ; cmd=cmd->next, i++)
		Con_Printf ("%s\n", cmd->name);
	Con_Printf ("%i commands\n", i);

/*
CmdList           : List all commands
CmdList [Partial] : List commands starting with 'Partial'
CmdList log [Partial] : Logs commands to file "cmdlist.txt" in the gamedir.
log cmdlist.txt wt  Couldn't open [%s] for writing!
    Command List
--------------
    %-16.16s
   %-16.16s
   %-16.16s
   %-16.16s
   --------------
%3i Commands for [%s]
CmdList ? for syntax
  --------------
%3i Total Commands
CmdList ? for syntax
*/
};