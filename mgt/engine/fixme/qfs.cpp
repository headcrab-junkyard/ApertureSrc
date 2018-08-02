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

// if a packfile directory differs from this, it is assumed to be hacked
#define PAK0_COUNT 339
#define PAK0_CRC 32981

qboolean com_modified; // set true if using non-id files

/*
=============================================================================

QUAKE FILESYSTEM

=============================================================================
*/

int com_filesize;

//
// in memory
//

typedef struct
{
	char name[MAX_QPATH];
	int filepos, filelen;
} packfile_t;

typedef struct pack_s
{
	char filename[MAX_OSPATH];
	int handle;
	int numfiles;
	packfile_t *files;
} pack_t;

char com_cachedir[MAX_OSPATH];
char com_gamedir[MAX_OSPATH];

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
============
COM_WriteFile

The filename will be prefixed by the current game directory
============
*/
void COM_WriteFile(const char *filename, void *data, int len)
{
	IFile *handle;
	char name[MAX_OSPATH];

	sprintf(name, "%s/%s", com_gamedir, filename);

	handle = FS_FileOpenWrite(name);
	if(handle == -1)
	{
		Sys_Printf("COM_WriteFile: failed on %s\n", name);
		return;
	}

	Sys_Printf("COM_WriteFile: %s\n", name);
	FS_FileWrite(handle, data, len);
	FS_FileClose(handle);
}

/*
============
COM_CreatePath

Only used for CopyFile
============
*/
void COM_CreatePath(const char *path)
{
	char *ofs;

	for(ofs = (char *)path + 1; *ofs; ofs++)
	{
		if(*ofs == '/')
		{
			// create the directory
			*ofs = 0;
			FS_mkdir(path);
			*ofs = '/';
		}
	}
}

/*
===========
COM_CopyFile

Copies a file over from the net to the local cache, creating any directories
needed.  This is for the convenience of developers using ISDN from home.
===========
*/
void COM_CopyFile(const char *netpath, char *cachepath)
{
	IFile *in, *out;
	int remaining, count;
	char buf[4096];

	remaining = FS_FileOpenRead(netpath, &in);
	COM_CreatePath(cachepath); // create directories up to the cache file
	out = FS_FileOpenWrite(cachepath);

	while(remaining)
	{
		if(remaining < sizeof(buf))
			count = remaining;
		else
			count = sizeof(buf);
		FS_FileRead(in, buf, count);
		FS_FileWrite(out, buf, count);
		remaining -= count;
	}

	FS_FileClose(in);
	FS_FileClose(out);
}

/*
===========
COM_FindFile

Finds the file in the search path.
Sets com_filesize and one of handle or file
===========
*/
int COM_FindFile(const char *filename, int *handle, IFile **file)
{
	searchpath_t *search;
	char netpath[MAX_OSPATH];
	char cachepath[MAX_OSPATH];
	pack_t *pak;
	int i;
	int findtime, cachetime;

	if(file && handle)
		Sys_Error("COM_FindFile: both handle and file set");
	if(!file && !handle)
		Sys_Error("COM_FindFile: neither handle or file set");

	//
	// search through the path, one element at a time
	//
	search = com_searchpaths;

	for(; search; search = search->next)
	{
		// is the element a pak file?
		if(search->pack)
		{
			// look through all the pak file elements
			pak = search->pack;
			for(i = 0; i < pak->numfiles; i++)
				if(!strcmp(pak->files[i].name, filename))
				{ // found it!
					Sys_Printf("PackFile: %s : %s\n", pak->filename, filename);
					if(handle)
					{
						*handle = pak->handle;
						FS_FileSeek(pak->handle, pak->files[i].filepos);
					}
					else
					{ // open a new file on the pakfile
						*file = fopen(pak->filename, "rb");
						if(*file)
							fseek(*file, pak->files[i].filepos, SEEK_SET);
					}
					com_filesize = pak->files[i].filelen;
					return com_filesize;
				}
		}
		else
		{
			// check a file in the directory tree

			sprintf(netpath, "%s/%s", search->filename, filename);

			findtime = FS_FileTime(netpath);
			if(findtime == -1)
				continue;

			// see if the file needs to be updated in the cache
			if(!com_cachedir[0])
				strcpy(cachepath, netpath);
			else
			{
#if defined(_WIN32)
				if((strlen(netpath) < 2) || (netpath[1] != ':'))
					sprintf(cachepath, "%s%s", com_cachedir, netpath);
				else
					sprintf(cachepath, "%s%s", com_cachedir, netpath + 2);
#else
				sprintf(cachepath, "%s%s", com_cachedir, netpath);
#endif

				cachetime = FS_FileTime(cachepath);

				if(cachetime < findtime)
					COM_CopyFile(netpath, cachepath);
				strcpy(netpath, cachepath);
			}

			Sys_Printf("FindFile: %s\n", netpath);
			com_filesize = FS_FileOpenRead(netpath, &i);
			if(handle)
				*handle = i;
			else
			{
				FS_FileClose(i);
				*file = fopen(netpath, "rb");
			}
			return com_filesize;
		}
	}

	Sys_Printf("FindFile: can't find %s\n", filename);

	if(handle)
		*handle = -1;
	else
		*file = nullptr;
	com_filesize = -1;
	return -1;
}

/*
===========
COM_OpenFile

filename never has a leading slash, but may contain directory walks
returns a handle and a length
it may actually be inside a pak file
===========
*/
int COM_OpenFile(const char *filename, int *handle)
{
	return COM_FindFile(filename, handle, nullptr);
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
============
COM_CloseFile

If it is a pak file handle, don't really close it
============
*/
void COM_CloseFile(int h)
{
	searchpath_t *s;

	for(s = com_searchpaths; s; s = s->next)
		if(s->pack && s->pack->handle == h)
			return;

	FS_FileClose(h);
}

/*
============
COM_LoadFile

Filename are reletive to the quake directory.
Allways appends a 0 byte.
============
*/
cache_user_t *loadcache;
byte *loadbuf;
int loadsize;
byte *COM_LoadFile(const char *path, int usehunk)
{
	int h;
	byte *buf;
	char base[32];
	int len;

	buf = nullptr; // quiet compiler warning

	// look for it in the filesystem or pack files
	len = COM_OpenFile(path, &h);
	if(h == -1)
		return nullptr;

	// extract the filename base name for hunk tag
	COM_FileBase(path, base);

	if(usehunk == 1)
		buf = (byte*)Hunk_AllocName(len + 1, base);
	else if(usehunk == 2)
		buf = (byte*)Hunk_TempAlloc(len + 1);
	else if(usehunk == 0)
		buf = (byte*)Z_Malloc(len + 1);
	else if(usehunk == 3)
		buf = (byte*)Cache_Alloc(loadcache, len + 1, base);
	else if(usehunk == 4)
	{
		if(len + 1 > loadsize)
			buf = (byte*)Hunk_TempAlloc(len + 1);
		else
			buf = loadbuf;
	}
	else
		Sys_Error("COM_LoadFile: bad usehunk");

	if(!buf)
		Sys_Error("COM_LoadFile: not enough space for %s", path);

	((byte *)buf)[len] = 0;

	Draw_BeginDisc();
	FS_FileRead(h, buf, len);
	COM_CloseFile(h);
	Draw_EndDisc();

	return buf;
}

byte *COM_LoadHunkFile(const char *path)
{
	return COM_LoadFile(path, 1);
}

byte *COM_LoadTempFile(const char *path)
{
	return COM_LoadFile(path, 2);
}

void COM_LoadCacheFile(const char *path, struct cache_user_s *cu)
{
	loadcache = cu;
	COM_LoadFile(path, 3);
}

// uses temp hunk if larger than bufsize
byte *COM_LoadStackFile(const char *path, void *buffer, int bufsize)
{
	byte *buf;

	loadbuf = (byte *)buffer;
	loadsize = bufsize;
	buf = COM_LoadFile(path, 4);

	return buf;
}

/*
=================
COM_LoadPackFile

Takes an explicit (not game tree related) path to a pak file.

Loads the header and directory, adding the files at the beginning
of the list so they override previous pack files.
=================
*/
pack_t *COM_LoadPackFile(const char *packfile)
{
	dpackheader_t header;
	int i;
	packfile_t *newfiles;
	int numpackfiles;
	pack_t *pack;
	int packhandle;
	dpackfile_t info[MAX_FILES_IN_PACK];
	unsigned short crc;

	if(FS_FileOpenRead(packfile, &packhandle) == -1)
	{
		//              Con_Printf ("Couldn't open %s\n", packfile);
		return nullptr;
	}
	FS_FileRead(packhandle, (void *)&header, sizeof(header));
	if(header.id[0] != 'P' || header.id[1] != 'A' || header.id[2] != 'C' || header.id[3] != 'K')
		Sys_Error("%s is not a packfile", packfile);
	header.dirofs = LittleLong(header.dirofs);
	header.dirlen = LittleLong(header.dirlen);

	numpackfiles = header.dirlen / sizeof(dpackfile_t);

	if(numpackfiles > MAX_FILES_IN_PACK)
		Sys_Error("%s has %i files", packfile, numpackfiles);

	if(numpackfiles != PAK0_COUNT)
		com_modified = true; // not the original file

	newfiles = (packfile_t*)Hunk_AllocName(numpackfiles * sizeof(packfile_t), "packfile");

	FS_FileSeek(packhandle, header.dirofs);
	FS_FileRead(packhandle, (void *)info, header.dirlen);

	// crc the directory to check for modifications
	CRC_Init(&crc);
	for(i = 0; i < header.dirlen; i++)
		CRC_ProcessByte(&crc, ((byte *)info)[i]);
	if(crc != PAK0_CRC)
		com_modified = true;

	// parse the directory
	for(i = 0; i < numpackfiles; i++)
	{
		strcpy(newfiles[i].name, info[i].name);
		newfiles[i].filepos = LittleLong(info[i].filepos);
		newfiles[i].filelen = LittleLong(info[i].filelen);
	}

	pack = (pack_t*)Hunk_Alloc(sizeof(pack_t));
	strcpy(pack->filename, packfile);
	pack->handle = packhandle;
	pack->numfiles = numpackfiles;
	pack->files = newfiles;

	Con_Printf("Added packfile %s (%i files)\n", packfile, numpackfiles);
	return pack;
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
		com_modified = true;
		COM_AddGameDirectory(va("%s/%s", basedir, com_argv[i + 1]));
	}

	//
	// -path <dir or packfile> [<dir or packfile>] ...
	// Fully specifies the exact serach path, overriding the generated one
	//
	i = COM_CheckParm("-path");
	if(i)
	{
		com_modified = true;
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
		}
	}
}