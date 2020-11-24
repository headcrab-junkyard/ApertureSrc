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
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

sizebuf_t cmd_text{};

/*
============
Cbuf_Init
============
*/
void Cbuf_Init()
{
	SZ_Alloc(&cmd_text, 8192); // space for commands and script files
}

/*
============
Cbuf_AddText

Adds command text at the end of the buffer
============
*/
void Cbuf_AddText(const char *text)
{
	int l = Q_strlen(text);

	if(cmd_text.cursize + l >= cmd_text.maxsize)
	{
		gpSystem->Printf("Cbuf_AddText: overflow\n");
		return;
	};

	cmd_text.Write(text, Q_strlen(text));
};

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void Cbuf_InsertText(const char *text)
{
	char *temp{nullptr};

	// copy off any commands still remaining in the exec buffer
	int templen = cmd_text.cursize;
	if(templen)
	{
		temp = (char *)Z_Malloc(templen);
		Q_memcpy(temp, cmd_text.data, templen);
		cmd_text.Clear();
	};

	// add the entire text of the file
	Cbuf_AddText(text);

	// add the copied off data
	if(templen)
	{
		cmd_text.Write(temp, templen);
		Z_Free(temp);
	};
};

/*
============
Cbuf_Execute
============
*/
void Cbuf_Execute()
{
	int i;
	char *text;
	char line[1024];
	int quotes;

	while(cmd_text.cursize)
	{
		// find a \n or ; line break
		text = (char *)cmd_text.data;

		quotes = 0;
		for(i = 0; i < cmd_text.cursize; i++)
		{
			if(text[i] == '"')
				quotes++;
			if(!(quotes & 1) && text[i] == ';')
				break; // don't break if inside a quoted string
			if(text[i] == '\n')
				break;
		};

		Q_memcpy(line, text, i);
		line[i] = 0;

		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec, alias) can insert data at the
		// beginning of the text buffer

		if(i == cmd_text.cursize)
			cmd_text.cursize = 0;
		else
		{
			i++;
			cmd_text.cursize -= i;
			Q_memcpy(text, text + i, cmd_text.cursize);
		};

		// execute the command line
		Cmd_ExecuteString(line, src_command);

		if(cmd_wait)
		{ // skip out while text still remains in buffer, leaving it
			// for next frame
			cmd_wait = false;
			break;
		};
	};
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

#define MAX_ARGS 80

static int cmd_argc;
static char *cmd_argv[MAX_ARGS];
static const char *cmd_null_string = "";
static const char *cmd_args = nullptr;

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
Cmd_Argc
============
*/
int Cmd_Argc()
{
	return cmd_argc;
};

/*
============
Cmd_Argv
============
*/
const char *Cmd_Argv(int arg)
{
	if((unsigned)arg >= cmd_argc)
		return cmd_null_string;
	return cmd_argv[arg];
};

/*
============
Cmd_Args
============
*/
const char *Cmd_Args()
{
	return cmd_args;
};

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
============
*/
void Cmd_TokenizeString(const char *text)
{
	int i;

	// clear the args from the last string
	for(i = 0; i < cmd_argc; i++)
		Z_Free(cmd_argv[i]);

	cmd_argc = 0;
	cmd_args = nullptr;

	while(1)
	{
		// skip whitespace up to a /n
		while(*text && *text <= ' ' && *text != '\n')
			text++;

		if(*text == '\n')
		{
			// a newline seperates commands in the buffer
			text++;
			break;
		};

		if(!*text)
			return;

		if(cmd_argc == 1)
			cmd_args = text;

		text = COM_Parse(text);
		if(!text)
			return;

		if(cmd_argc < MAX_ARGS)
		{
			cmd_argv[cmd_argc] = (char *)Z_Malloc(Q_strlen(com_token) + 1);
			Q_strcpy(cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}
}

/*
============
Cmd_AddCommand
============
*/
void Cmd_AddCommand(const char *cmd_name, xcommand_t function)
{
	cmd_function_t *cmd;

	if(host_initialized) // because hunk allocation would get stomped
		Sys_Error("Cmd_AddCommand after host_initialized");

	// fail if the command is a variable name
	if(Cvar_VariableString(cmd_name)[0])
	{
		gpSystem->Printf("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		return;
	}

	// fail if the command already exists
	for(cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if(!Q_strcmp(cmd_name, cmd->name))
		{
			gpSystem->Printf("Cmd_AddCommand: %s already defined\n", cmd_name);
			return;
		}
	}

	cmd = (cmd_function_t *)Hunk_Alloc(sizeof(cmd_function_t));
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
}

/*
============
Cmd_Exists
============
*/
qboolean Cmd_Exists(const char *cmd_name)
{
	for(auto cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if(!Q_strcmp(cmd_name, cmd->name))
			return true;
	};

	return false;
}

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
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void Cmd_ExecuteString(const char *text, cmd_source_t src)
{
	cmd_function_t *cmd;
	cmdalias_t *a;

	cmd_source = src;
	
	CCmdArgs Args(text);
	//Cmd_TokenizeString(text);

	// execute the command line
	
	// no tokens
	if(!Args.GetCount())
		return;

	// check functions
	for(cmd = cmd_functions; cmd; cmd = cmd->next)
	{
		if(!Q_strcasecmp(Args.GetByIndex(0), cmd->name))
		{
			cmd->function(Args);
			return;
		};
	};

	// check alias
	for(a = cmd_alias; a; a = a->next)
	{
		if(!Q_strcasecmp(Args.GetByIndex(0), a->name))
		{
			Cbuf_InsertText(a->value);
			return;
		};
	};

	// check cvars
	if(!Cvar_Command())
		gpSystem->Printf("Unknown command \"%s\"\n", Args.GetByIndex(0));
};

/*
================
Cmd_CheckParm

Returns the position (1 to argc-1) in the command's argument list
where the given parameter apears, or 0 if not present
================
*/
int Cmd_CheckParm(const char *parm)
{
	int i;

	if(!parm)
		gpSystem->Error("Cmd_CheckParm: NULL");

	for(i = 1; i < Cmd_Argc(); i++)
		if(!Q_strcasecmp(parm, Cmd_Argv(i)))
			return i;

	return 0;
}