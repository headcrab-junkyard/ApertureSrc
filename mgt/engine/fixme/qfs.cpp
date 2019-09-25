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
