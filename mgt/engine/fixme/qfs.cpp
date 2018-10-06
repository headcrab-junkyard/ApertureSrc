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

#include "quakedef.h"

/*
=============================================================================

ENGINE FILESYSTEM

=============================================================================
*/

/*
============
COM_Path_f

============
*/
void COM_Path_f()
{
	searchpath_t *s;

	Con_Printf("Current search path:\n");
	for(s = com_searchpaths; s; s = s->next)
	{
		if(s->pack)
		{
			Con_Printf("%s (%i files)\n", s->pack->filename, s->pack->numfiles);
		}
		else
			Con_Printf("%s\n", s->filename);
	}
}

/*
===========
COM_FOpenFile

If the requested file is inside a packfile, a new FILE * will be opened
into the file.
===========
*/
int COM_FOpenFile(const char *filename, IFile **file)
{
	return COM_FindFile(filename, nullptr, file);
}

/*
================
COM_AddGameDirectory

Sets com_gamedir, adds the directory to the head of the path,
then loads and adds pak1.pak pak2.pak ... 
================
*/
void COM_AddGameDirectory(const char *dir)
{
	int i;
	searchpath_t *search;
	pack_t *pak;
	char pakfile[MAX_OSPATH];

	strcpy(com_gamedir, dir);

	//
	// add the directory to the search path
	//
	search = (searchpath_t*)Hunk_Alloc(sizeof(searchpath_t));
	strcpy(search->filename, dir);
	search->next = com_searchpaths;
	com_searchpaths = search;

	//
	// add any pak files in the format pak0.pak pak1.pak, ...
	//
	for(i = 0;; i++)
	{
		sprintf(pakfile, "%s/pak%i.pak", dir, i);
		pak = COM_LoadPackFile(pakfile);
		if(!pak)
			break;
		search = (searchpath_t*)Hunk_Alloc(sizeof(searchpath_t));
		search->pack = pak;
		search->next = com_searchpaths;
		com_searchpaths = search;
	}

	//
	// add the contents of the parms.txt file to the end of the command line
	//
}

/*
================
COM_InitFilesystem
================
*/
void COM_InitFilesystem()
{
	int i, j;
	char basedir[MAX_OSPATH];
	searchpath_t *search;

	//
	// -basedir <path>
	// Overrides the system supplied base directory (under GAMENAME)
	//
	i = COM_CheckParm("-basedir");
	if(i && i < com_argc - 1)
		strcpy(basedir, com_argv[i + 1]);
	else
		strcpy(basedir, host_parms.basedir);

	j = strlen(basedir);

	if(j > 0)
	{
		if((basedir[j - 1] == '\\') || (basedir[j - 1] == '/'))
			basedir[j - 1] = 0;
	}

	//
	// -cachedir <path>
	// Overrides the system supplied cache directory (nullptr or /qcache)
	// -cachedir - will disable caching.
	//
	i = COM_CheckParm("-cachedir");
	if(i && i < com_argc - 1)
	{
		if(com_argv[i + 1][0] == '-')
			com_cachedir[0] = 0;
		else
			strcpy(com_cachedir, com_argv[i + 1]);
	}
	else if(host_parms.cachedir)
		strcpy(com_cachedir, host_parms.cachedir);
	else
		com_cachedir[0] = 0;

	//
	// start up with GAMENAME by default (valve)
	//
	COM_AddGameDirectory(va("%s/" GAMENAME, basedir));

	//
	// -game <gamedir>
	// Adds basedir/gamedir as an override game
	//
	i = COM_CheckParm("-game");
	if(i && i < com_argc - 1)
	{
		COM_AddGameDirectory(va("%s/%s", basedir, com_argv[i + 1]));
	}

	//
	// -path <dir or packfile> [<dir or packfile>] ...
	// Fully specifies the exact serach path, overriding the generated one
	//
	i = COM_CheckParm("-path");
	if(i)
	{
		com_searchpaths = nullptr;
		while(++i < com_argc)
		{
			if(!com_argv[i] || com_argv[i][0] == '+' || com_argv[i][0] == '-')
				break;

			search = (searchpath_t*)Hunk_Alloc(sizeof(searchpath_t));
			if(!strcmp(COM_FileExtension(com_argv[i]), "pak"))
			{
				search->pack = COM_LoadPackFile(com_argv[i]);
				if(!search->pack)
					Sys_Error("Couldn't load packfile: %s", com_argv[i]);
			}
			else
				strcpy(search->filename, com_argv[i]);
			search->next = com_searchpaths;
			com_searchpaths = search;
		};
	};
};