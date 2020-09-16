/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020 BlackPhrase
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
/// @brief file i/o

//#include <cstdio>
//#include <cstdarg>
#include <cstring>
//#include <cstdlib>
//#include <sys/stat.h>
#include "FileSystem.hpp"
#include "File.hpp"

// TODO

// .pak support
#define MAX_FILES_IN_PACK 2048

#ifndef MAX_QPATH
const int MAX_QPATH = 64;
#endif

#ifndef MAX_OSPATH
const int MAX_OSPATH = 256;
#endif

//
// on disk
//

typedef struct
{
	char name[56]{};
	int filepos{0}, filelen{0};
} dpackfile_t;

typedef struct
{
	char id[4]{};
	int dirofs{0};
	int dirlen{0};
} dpackheader_t;
//






//
// in memory
//

typedef struct
{
	char name[MAX_QPATH]{};
	int filepos{0}, filelen{0};
} packfile_t;

typedef struct pack_s
{
	char filename[MAX_OSPATH]{};
	IFile *handle{nullptr}; // FILE *handle; // TODO
	int numfiles{0};
	packfile_t *files{nullptr};
} pack_t;

// search paths
typedef struct searchpath_s
{
	char filename[MAX_OSPATH];
	pack_t *pack; // only one of filename / pack will be used
	struct searchpath_s *next;
} searchpath_t;

searchpath_t *com_searchpaths{nullptr};

int com_filesize{0};

char com_cachedir[MAX_OSPATH]{};

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

	newfiles = (packfile_t*)Hunk_AllocName(numpackfiles * sizeof(packfile_t), "packfile");

	FS_FileSeek(packhandle, header.dirofs);
	FS_FileRead(packhandle, (void *)info, header.dirlen);

	// crc the directory to check for modifications
	CRC_Init(&crc);
	
	for(i = 0; i < header.dirlen; i++)
		CRC_ProcessByte(&crc, ((byte *)info)[i]);

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
	};

	//
	// add the contents of the parms.txt file to the end of the command line
	//
};

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
	COM_AddGameDirectory(path, "", false);
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
IFile *COM_FindFile(const char *filename, int *handle, IFile **file)
{
	searchpath_t *search;
	char netpath[MAX_OSPATH];
	char cachepath[MAX_OSPATH];
	pack_t *pak;
	int i;
	int findtime, cachetime;
	IFile *pFile;

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
						pak->handle->Seek(pak->files[i].filepos);
					}
					else
					{
						// open a new file on the pakfile
						pFile = FileOpen(pak->filename, "rb");
						if(pFile)
							pFile->Seek(pak->files[i].filepos, SEEK_SET);
					}
					com_filesize = pak->files[i].filelen;
					return pFile;
				}
		}
		else
		{
			// check a file in the directory tree

			sprintf(netpath, "%s/%s", search->filename, filename);

			findtime = GetFileTime(netpath);
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

				cachetime = GetFileTime(cachepath);

				if(cachetime < findtime)
					COM_CopyFile(netpath, cachepath);
				strcpy(netpath, cachepath);
			}

			Sys_Printf("FindFile: %s\n", netpath);
			pFile = OpenFile(netpath, "rb");
			com_filesize = pFile->GetSize();
			return pFile;
		}
	}

	Sys_Printf("FindFile: can't find %s\n", filename);
	com_filesize = -1;
	return nullptr;
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
	IFile *pFile{new CFile(asName, asMode)};
	//mlstOpenHandles.push_back(pFile);
	//return COM_FindFile(asName, nullptr, nullptr); // TODO
	return pFile;
};

/*
============
COM_CloseFile

If it is a pak file handle, don't really close it
============
*/
void CFileSystem::CloseFile(IFile *apFile)
{
	// If it is a pak file handle, don't really close it
	for(searchpath_t *s = com_searchpaths; s; s = s->next)
		if(s->pack && s->pack->handle == apFile)
			return;
	
	if(!apFile)
		return;
	
	// TODO: free the file
	
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

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int CFileSystem::GetFileTime(const char *path) const
{
#ifdef _WIN32
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
#else
	struct stat buf;

	if(stat(path, &buf) == -1)
		return -1;

	return buf.st_mtime;
#endif
};

int CFileSystem::GetFileSize(const char *path) const
{
	return 0; //filelength();
};

int CFileSystem::findhandle()
{
	for(int i = 1; i < MAX_HANDLES; i++)
		if(!sys_handles[i]) // TODO: if(!sys_handles[i].hFile) for sun
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