/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020-2022 BlackPhrase
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

#include "FileSystem.hpp"
#include "File.hpp"

namespace fs = std::filesystem;

// .pak support
constexpr auto MAX_FILES_IN_PACK{2048};

//
// on disk
//

struct dpackfile_t
{
	char name[56]{};
	int filepos{0}, filelen{0};
};

struct dpackheader_t
{
	char id[4]{};
	int dirofs{0};
	int dirlen{0};
};

CFileSystem gFileSystem;
CFileSystem *gpFileSystem{&gFileSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CFileSystem, IFileSystem, OGS_FILESYSTEM_INTERFACE_VERSION, gFileSystem);

CFileSystem::CFileSystem() = default;
CFileSystem::~CFileSystem() = default;

void CFileSystem::Mount()
{
};

void CFileSystem::Unmount()
{
	RemoveAllSearchPaths();
	
	// TODO
};

void CFileSystem::AddSearchPath(const char *asPath, const char *asAlias, bool abReadOnly)
{
	//mlstSearchPaths.emplace_back(asAlias, asPath, abReadOnly); // TODO
	AddGameDirectory(asPath, "", false);
};

bool CFileSystem::RemoveSearchPath(const char *asAlias)
{
	return false;
};

void CFileSystem::RemoveAllSearchPaths()
{
};

IFile *CFileSystem::OpenPathID(const char *asPath, const char *asPathID)
{
	return nullptr;
};

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

void CFileSystem::CloseFile(IFile *apFile)
{
};

void CFileSystem::RemoveFile(const char *asRelativePath, const char *asAlias)
{
	fs::remove(mRootDir/asRelativePath); // TODO: remove_all?
};

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int CFileSystem::GetFileTime(const char *asPath) const
{
#ifdef _WIN32
	//int t = 0; //VID_ForceUnlockedAndReturnState(); // TODO windows
	auto f{fopen(asPath, "rb")};
	//int retval;
	
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

	if(stat(asPath, &buf) == -1)
		return -1;

	return buf.st_mtime;
#endif
};

uint CFileSystem::GetFileSize(const char *asPath) const
{
	//return filelength();
	return fs::file_size(mRootDir/asPath);
};

bool CFileSystem::IsFileExists(const char *asFileName) const
{
	return fs::exists(mRootDir/asFileName);
};

bool CFileSystem::IsDirectory(const char *asFileName) const
{
	return fs::is_directory(mRootDir/asFileName);
};

/*
=================
COM_LoadPackFile

Takes an explicit (not game tree related) path to a pak file.

Loads the header and directory, adding the files at the beginning
of the list so they override previous pack files.
=================
*/
pack_t *CFileSystem::LoadPackFile(const char *packfile)
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
	
	pack = reinterpret_cast<pack_t*>(Hunk_Alloc(sizeof(pack_t)));
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
void CFileSystem::AddGameDirectory(const char *asDir)
{
	strcpy(com_gamedir, asDir);
	
	//
	// add the directory to the search path
	//
	auto search{reinterpret_cast<searchpath_t*>(Hunk_Alloc(sizeof(searchpath_t)))};
	strcpy(search->filename, asDir);
	search->next = com_searchpaths;
	com_searchpaths = search;
	
	pack_t *pak{nullptr};
	char pakfile[MAX_OSPATH];
	
	//
	// add any pak files in the format pak0.pak pak1.pak, ...
	//
	for(int i = 0; ; ++i)
	{
		sprintf(pakfile, "%s/pak%i.pak", asDir, i);
		pak = COM_LoadPackFile(pakfile);
		if(!pak)
			break;
		search = reinterpret_cast<searchpath_t*>(Hunk_Alloc(sizeof(searchpath_t)));
		search->pack = pak;
		search->next = com_searchpaths;
		com_searchpaths = search;
	};
	
	//
	// add the contents of the parms.txt file to the end of the command line
	//
};