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

class CCmd
{
public:
	void Init();

	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	void AddCommand(const char *cmd_name, xcommand_t function);

	/// used by the cvar code to check for cvar / command name overlap
	bool Exists(const char *cmd_name) const;

	/// attempts to match a partial command for automatic command line completion
	/// returns nullptr if nothing fits
	const char *CompleteCommand(const char *partial) const;

	/// The functions that execute commands get their parameters with these
	/// functions. Cmd_Argv () will return an empty string, not a nullptr
	/// if arg > argc, so string operations are allways safe
	int GetArgc() const;
	const char *GetArgv(int arg) const;
	const char *GetArgs() const;

	/// Returns the position (1 to argc-1) in the command's argument list
	/// where the given parameter apears, or 0 if not present
	int CheckParm(const char *parm) const;

	/// Takes a null terminated string.
	/// Does not need to be /n terminated.
	/// Breaks the string up into arg tokens
	void TokenizeString(const char *text);

	/// Parses a single line of text into arguments and tries to execute it.
	/// The text can come from the command buffer, a remote client, or stdin
	void ExecuteString(const char *text, cmd_source_t src);

	/// adds the current command line as a clc_stringcmd to the client message.
	/// things like godmode, noclip, etc, are commands directed to the server,
	/// so when they are typed in at the console, they will need to be forwarded
	void ForwardToServer();

	/// used by command functions to send output to either the graphics console or
	/// passed as a print message to the client
	void Print(const char *text);
};