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

typedef struct pack_s pack_t;

// search paths
typedef struct searchpath_s
{
	char filename[MAX_OSPATH];
	pack_t *pack; // only one of filename / pack will be used
	struct searchpath_s *next;
} searchpath_t;

searchpath_t *com_searchpaths;
//
*/

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

IFile *CFileSystem::OpenFile(const char *asName)
{
	IFile *pFile{nullptr}; //new CFile(asName);
	//mlstOpenHandles.push_back(pFile);
	return pFile;
};

void CFileSystem::CloseFile(IFile *apFile)
{
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