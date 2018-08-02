/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
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
/// @brief misc functions used in client and server

#include "quakedef.h"

const int NUM_SAFE_ARGVS = 7;

static const char *largv[MAX_NUM_ARGVS + NUM_SAFE_ARGVS + 1];
static const char *argvdummy = " ";

static const char *safeargvs[NUM_SAFE_ARGVS] =
{ "-stdvid", "-nolan", "-nosound", "-nocdaudio", "-nojoy", "-nomouse", "-dibonly" };

cvar_t cmdline = { "cmdline", "0", false, true };

bool msg_suppress_1 = false;

void COM_InitFilesystem();

char com_token[1024];
int com_argc;
char **com_argv;

#define CMDLINE_LENGTH 256
char com_cmdline[CMDLINE_LENGTH];

/*


All of engine's data access is through a hierchal file system, but the contents of the file system can be transparently merged from several sources.

The "base directory" is the path to the directory holding the quake.exe and all game directories.  The sys_* files pass this to host_init in quakeparms_t->basedir.  This can be overridden with the "-basedir" command line parm to allow code debugging in a different directory.  The base directory is
only used during filesystem initialization.

The "game directory" is the first tree on the search path and directory that all generated files (savegames, screenshots, demos, config files) will be saved to.  This can be overridden with the "-game" command line parameter.  The game directory can never be changed while quake is executing.  This is a precacution against having a malicious server instruct clients to write files over areas they shouldn't.

The "cache directory" is only used during development to save network bandwidth, especially over ISDN / T1 lines.  If there is a cache directory
specified, when a file is found by the normal search path, it will be mirrored
into the cache directory, then opened there.



FIXME:
The file "parms.txt" will be read out of the game directory and appended to the current command line arguments to allow different games to initialize startup parms differently.  This could be used to add a "-sspeed 22050" for the high quality sound edition.  Because they are added at the end, they will not override an explicit setting on the original command line.
	
*/

//============================================================================

// ClearLink is used for new headnodes
void ClearLink(link_t *l)
{
	l->prev = l->next = l;
}

void RemoveLink(link_t *l)
{
	l->next->prev = l->prev;
	l->prev->next = l->next;
}

void InsertLinkBefore(link_t *l, link_t *before)
{
	l->next = before;
	l->prev = before->prev;
	l->prev->next = l;
	l->next->prev = l;
}
void InsertLinkAfter(link_t *l, link_t *after)
{
	l->next = after->next;
	l->prev = after;
	l->prev->next = l;
	l->next->prev = l;
}

/*
============================================================================

					BYTE ORDER FUNCTIONS

============================================================================
*/

qboolean bigendien;

short (*BigShort)(short l);
short (*LittleShort)(short l);
int (*BigLong)(int l);
int (*LittleLong)(int l);
float (*BigFloat)(float l);
float (*LittleFloat)(float l);

short ShortSwap(short l)
{
	byte b1, b2;

	b1 = l & 255;
	b2 = (l >> 8) & 255;

	return (b1 << 8) + b2;
}

short ShortNoSwap(short l)
{
	return l;
}

int LongSwap(int l)
{
	byte b1, b2, b3, b4;

	b1 = l & 255;
	b2 = (l >> 8) & 255;
	b3 = (l >> 16) & 255;
	b4 = (l >> 24) & 255;

	return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

int LongNoSwap(int l)
{
	return l;
}

float FloatSwap(float f)
{
	union
	{
		float f;
		byte b[4];
	} dat1, dat2;

	dat1.f = f;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];
	return dat2.f;
}

float FloatNoSwap(float f)
{
	return f;
}

//============================================================================

/*
============
COM_SkipPath
============
*/
const char *COM_SkipPath(const char *pathname)
{
	const char *last;

	last = pathname;
	while(*pathname)
	{
		if(*pathname == '/')
			last = pathname + 1;
		pathname++;
	}
	return last;
}

/*
============
COM_StripExtension
============
*/
void COM_StripExtension(const char *in, char *out)
{
	while(*in && *in != '.')
		*out++ = *in++;
	*out = 0;
}

/*
============
COM_FileExtension
============
*/
char *COM_FileExtension(const char *in)
{
	static char exten[8];
	int i;

	while(*in && *in != '.')
		in++;
	if(!*in)
		return "";
	in++;
	for(i = 0; i < 7 && *in; i++, in++)
		exten[i] = *in;
	exten[i] = 0;
	return exten;
}

/*
============
COM_FileBase
============
*/
void COM_FileBase(const char *in, char *out)
{
	const char *s, *s2;

	s = in + strlen(in) - 1;

	while(s != in && *s != '.')
		s--;

	for(s2 = s; *s2 && *s2 != '/'; s2--)
		;

	if(s - s2 < 2)
		strcpy(out, "?model?");
	else
	{
		s--;
		strncpy(out, s2 + 1, s - s2);
		out[s - s2] = 0;
	}
}

/*
==================
COM_DefaultExtension
==================
*/
void COM_DefaultExtension(char *path, const char *extension)
{
	char *src;
	//
	// if path doesn't have a .EXT, append extension
	// (extension should include the .)
	//
	src = path + strlen(path) - 1;

	while(*src != '/' && src != path)
	{
		if(*src == '.')
			return; // it has an extension
		src--;
	}

	strcat(path, extension);
}

/*
==============
COM_Parse

Parse a token out of a string
==============
*/
const char *COM_Parse(const char *data)
{
	int c;
	int len;

	len = 0;
	com_token[0] = 0;

	if(!data)
		return nullptr;

// skip whitespace
skipwhite:
	while((c = *data) <= ' ')
	{
		if(c == 0)
			return nullptr; // end of file;
		data++;
	}

	// skip // comments
	if(c == '/' && data[1] == '/')
	{
		while(*data && *data != '\n')
			data++;
		goto skipwhite;
	}

	// handle quoted strings specially
	if(c == '\"')
	{
		data++;
		while(1)
		{
			c = *data++;
			if(c == '\"' || !c)
			{
				com_token[len] = 0;
				return data;
			}
			com_token[len] = c;
			len++;
		}
	}

	// parse single characters
	if(c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':')
	{
		com_token[len] = c;
		len++;
		com_token[len] = 0;
		return data + 1;
	}

	// parse a regular word
	do
	{
		com_token[len] = c;
		data++;
		len++;
		c = *data;
		if(c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ':')
			break;
	} while(c > 32);

	com_token[len] = 0;
	return data;
}

/*
================
COM_CheckParm

Returns the position (1 to argc-1) in the program's argument list
where the given parameter apears, or 0 if not present
================
*/
int COM_CheckParm(const char *parm)
{
	int i;

	for(i = 1; i < com_argc; i++)
	{
		if(!com_argv[i])
			continue; // NEXTSTEP sometimes clears appkit vars.
		if(!Q_strcmp(parm, com_argv[i]))
			return i;
	}

	return 0;
}

void COM_CheckRegistered()
{
	// Check CD-key...

	Cvar_Set("cmdline", com_cmdline);
}

void COM_Path_f(const ICmdArgs &apArgs);

/*
================
COM_InitArgv
================
*/
void COM_InitArgv(int argc, char **argv)
{
	qboolean safe;
	int i, j, n;

	// reconstitute the command line for the cmdline externally visible cvar
	n = 0;

	for(j = 0; (j < MAX_NUM_ARGVS) && (j < argc); j++)
	{
		i = 0;

		while((n < (CMDLINE_LENGTH - 1)) && argv[j][i])
		{
			com_cmdline[n++] = argv[j][i++];
		}

		if(n < (CMDLINE_LENGTH - 1))
			com_cmdline[n++] = ' ';
		else
			break;
	}

	com_cmdline[n] = 0;

	safe = false;

	for(com_argc = 0; (com_argc < MAX_NUM_ARGVS) && (com_argc < argc);
	    com_argc++)
	{
		largv[com_argc] = argv[com_argc];
		if(!Q_strcmp("-safe", argv[com_argc]))
			safe = true;
	}

	if(safe)
	{
		// force all the safe-mode switches. Note that we reserved extra space in
		// case we need to add these, so we don't need an overflow check
		for(i = 0; i < NUM_SAFE_ARGVS; i++)
		{
			largv[com_argc] = safeargvs[i];
			com_argc++;
		}
	}

	largv[com_argc] = argvdummy;
	com_argv = (char**)largv;
}

/*
================
COM_Init
================
*/
void COM_Init(const char *basedir)
{
	byte swaptest[2] = { 1, 0 };

	// set the byte swapping variables in a portable manner
	if(*(short *)swaptest == 1)
	{
		bigendien = false;
		BigShort = ShortSwap;
		LittleShort = ShortNoSwap;
		BigLong = LongSwap;
		LittleLong = LongNoSwap;
		BigFloat = FloatSwap;
		LittleFloat = FloatNoSwap;
	}
	else
	{
		bigendien = true;
		BigShort = ShortNoSwap;
		LittleShort = ShortSwap;
		BigLong = LongNoSwap;
		LittleLong = LongSwap;
		BigFloat = FloatNoSwap;
		LittleFloat = FloatSwap;
	}

	Cvar_RegisterVariable(&cmdline);
	Cmd_AddCommand("path", COM_Path_f);

	COM_InitFilesystem();
	COM_CheckRegistered();
}

/*
============
va

does a varargs printf into a temp buffer, so I don't need to have
varargs versions of all text functions.
FIXME: make this buffer size safe someday
============
*/
char *va(const char *format, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, format);
	vsprintf(string, format, argptr);
	va_end(argptr);

	return string;
}

/// just for debugging
int memsearch(byte *start, int count, int search)
{
	int i;

	for(i = 0; i < count; i++)
		if(start[i] == search)
			return i;
	return -1;
}