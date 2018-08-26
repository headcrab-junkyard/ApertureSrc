/*
 *	This file is part of Magenta Engine
 *  Copyright (C) 1996-1997 Id Software, Inc.
 *	Copyright (C) 2018 BlackPhrase
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file
/// @brief file i/o

#include <cstring>
#include "FileSystem.hpp"

// if a packfile directory differs from this, it is assumed to be hacked
#define PAK0_COUNT 339
#define PAK0_CRC 32981

// TODO
/*
// .pak support
#define MAX_FILES_IN_PACK 2048

//
// on disk
//
typedef struct
{
	char name[56];
	int filepos, filelen;
} dpackfile_t;

typedef struct
{
	char id[4];
	int dirofs;
	int dirlen;
} dpackheader_t;
//
*/

#ifndef MAX_OSPATH
const int MAX_OSPATH = 256;
#endif

#ifndef MAX_QPATH
const int MAX_QPATH = 64;
#endif

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
	IFile *handle; // FILE *handle; // TODO
	int numfiles;
	packfile_t *files;
} pack_t;

// search paths
typedef struct searchpath_s
{
	char filename[MAX_OSPATH];
	pack_t *pack; // only one of filename / pack will be used
	struct searchpath_s *next;
} searchpath_t;

searchpath_t *com_searchpaths;

int com_filesize;

char com_cachedir[MAX_OSPATH];

// TODO: temp
void Sys_Error(const char *msg, ...)
{
};

/*
=================
COM_LoadPackFile

Takes an explicit (not game tree related) path to a pak file.

Loads the header and directory, adding the files at the beginning
of the list so they override previous pack files.
=================
*/
// TODO
/*
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
		//Con_Printf("Couldn't open %s\n", packfile);
		return nullptr;
	};
	
	FS_FileRead(packhandle, (void *)&header, sizeof(header));
	
	if(header.id[0] != 'P' || header.id[1] != 'A' || header.id[2] != 'C' || header.id[3] != 'K')
		Sys_Error("%s is not a packfile", packfile);
	
	header.dirofs = LittleLong(header.dirofs);
	header.dirlen = LittleLong(header.dirlen);

	numpackfiles = header.dirlen / sizeof(dpackfile_t);

	if(numpackfiles > MAX_FILES_IN_PACK)
		Sys_Error("%s has %i files", packfile, numpackfiles);

	// TODO
	//if(numpackfiles != PAK0_COUNT)
		//com_modified = true; // not the original file

	newfiles = (packfile_t*)Hunk_AllocName(numpackfiles * sizeof(packfile_t), "packfile");

	FS_FileSeek(packhandle, header.dirofs);
	FS_FileRead(packhandle, (void *)info, header.dirlen);

	// crc the directory to check for modifications
	CRC_Init(&crc);
	
	for(i = 0; i < header.dirlen; i++)
		CRC_ProcessByte(&crc, ((byte *)info)[i]);
	
	// TODO
	//if(crc != PAK0_CRC)
		//com_modified = true;

	// parse the directory
	for(i = 0; i < numpackfiles; i++)
	{
		strcpy(newfiles[i].name, info[i].name);
		newfiles[i].filepos = LittleLong(info[i].filepos);
		newfiles[i].filelen = LittleLong(info[i].filelen);
	};

	pack = (pack_t*)Hunk_Alloc(sizeof(pack_t));
	strcpy(pack->filename, packfile);
	pack->handle = packhandle;
	pack->numfiles = numpackfiles;
	pack->files = newfiles;

	Con_Printf("Added packfile %s (%i files)\n", packfile, numpackfiles);
	return pack;
};
*/

EXPOSE_SINGLE_INTERFACE(CFileSystem, IFileSystem, MGT_FILESYSTEM_INTERFACE_VERSION);

CFileSystem::CFileSystem() = default;
CFileSystem::~CFileSystem() = default;

void CFileSystem::Mount()
{
};

void CFileSystem::Unmount()
{
	RemoveAllSearchPaths();
};

void CFileSystem::AddSearchPath(const char *path, const char *alias, bool nowrite)
{
	//mlstSearchPaths.emplace_back(alias, path, nowrite); // TODO
};

void CFileSystem::RemoveAllSearchPaths()
{
};

IFile *CFileSystem::OpenPathID(const char *asFilePath, const char *asPathID)
{
	// TODO
	return nullptr;
};

/*
===========
COM_FindFile

Finds the file in the search path.
Sets com_filesize and one of handle or file
===========
*/
// TODO
/*
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
*/

/*
===========
COM_OpenFile

filename never has a leading slash, but may contain directory walks
returns a handle
it may actually be inside a pak file
===========
*/
IFile *CFileSystem::OpenFile(const char *asName, const char *asMode)
{
	IFile *pFile{nullptr}; //new CFile(asName, asMode);
	//mlstOpenHandles.push_back(pFile);
	//return COM_FindFile(asName, nullptr, nullptr); // TODO
	return pFile;
};

void CFileSystem::CloseFile(IFile *apFile)
{
	// If it is a pak file handle, don't really close it
	for(searchpath_t *s = com_searchpaths; s; s = s->next)
		if(s->pack && s->pack->handle == apFile)
			return;
	
	if(!apFile)
		return;
	
	// TODO
	
	//auto It{mlstOpenHandles.find(apFile)};
	
	//if(It)
	{
		//delete It;
		//mlstOpenHandles.erase(It);
	};
};

/*
int CFileSystem::FileOpen(const char *path, const char *mode)
{
	// FS_FileOpenRead
	//int Sys_FileOpenRead(char *path, int *hndl)
	/*
	FILE *f;
	int i, retval;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1; // retval = -1 // TODO
	}
	else
	{
		sys_handles[i] = f;
		*hndl = i;
		retval = filelength(f);
	};
	
	//VID_ForceLockState(t); // TODO
	
	return retval;
	
	*/
	//
	
	// TODO
	/*
	//int t{VID_ForceUnlockedAndReturnState()}; // TODO: non-dedicated win only
	
	FILE *f{fopen(path, "wb")};
	
	if(!f)
		Sys_Error("Error opening %s: %s", path, strerror(errno));
	
	int i{findhandle()};
	
	sys_handles[i] = f;
	
	//VID_ForceLockState(t); // TODO: non-dedicated win only
	
	return i;
};
*/

/*
void CFileSystem::FileClose(int handle)
{
	if(!handle)
		return;

	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
#ifdef _WIN32
	fclose(sys_handles[handle]);
	sys_handles[handle] = nullptr;
#else
	close(handle);
#endif

	//VID_ForceLockState(t); // TODO: windows & not dedicated
};
*/

void CFileSystem::FileSeek(int handle, int position) // TODO: seek mode
{
	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
#ifdef _WIN32
	fseek(sys_handles[handle], position, SEEK_SET);
#else
	lseek(handle, position, SEEK_SET);
#endif

	//VID_ForceLockState(t); // TODO: windows & not dedicated
};

int CFileSystem::FileRead(int handle, void *dest, int count)
{
	if(!dest)
		return -1;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	int x;
	
#ifdef _WIN32
	x = fread(dest, 1, count, sys_handles[handle]);
#else
	x = read(handle, dest, count);
#endif

	//VID_ForceLockState(t); // TODO
	return x;
};

int CFileSystem::FileWrite(int handle, const void *data, int count)
{
	if(!data)
		return -1;
	
	//int t = VID_ForceUnlockedAndReturnState(); // TODO
	
	int x;
	
#ifdef _WIN32
	x = fwrite(data, 1, count, sys_handles[handle]);
#else
	x = write(handle, src, count);
#endif

	//VID_ForceLockState(t); // TODO
	return x;
};

int CFileSystem::GetFileTime(const char *path) const
{
	int t = 0; //VID_ForceUnlockedAndReturnState(); // TODO windows
	FILE *f{fopen(path, "rb")};
	int retval;
	
	if(f)
	{
		fclose(f);
		return 1; // TODO: retval = 1;
	}
	//else // TODO
		//retval = -1; // TODO
	
	//VID_ForceLockState(t); // TODO windows
	return -1; //return retval; // TODO
};

int CFileSystem::GetFileSize(const char *path) const
{
	return 0; //filelength();
};

int CFileSystem::findhandle()
{
	for(int i = 1; i < MAX_HANDLES; i++)
		if(!sys_handles[i])
			return i;
	
	Sys_Error("out of handles");
	return -1;
};

/*
================
filelength
================
*/
int CFileSystem::filelength(FILE *f)
{
	//int t = VID_ForceUnlockedAndReturnState(); // TODO: windows & not dedicated
	
	int pos = ftell(f);
	fseek(f, 0, SEEK_END);
	
	int end = ftell(f);
	fseek(f, pos, SEEK_SET);

	//VID_ForceLockState(t); // TODO: windows & not dedicated
	
	return end;
};