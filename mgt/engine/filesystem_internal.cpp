/*
*	This file is part of Magenta Engine
*
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
/// @brief

#include "quakedef.h"
#include "IFileSystem.hpp"

IFileSystem *gpFileSystem{nullptr};

void FileSystem_Init(const char *basedir, void *filesystemFactory)
{
	// TODO
	
	if(!basedir || !*basedir)
		return;
	
	if(!filesystemFactory)
		return;
	
	gpFileSystem = (IFileSystem*)(((CreateInterfaceFn)filesystemFactory)(OGS_FILESYSTEM_INTERFACE_VERSION, nullptr)); // TODO: FILESYSTEM_INTERFACE_VERSION
	
	if(!gpFileSystem)
		return;
};

void FileSystem_Shutdown()
{
	// TODO
	
	gpFileSystem = nullptr;
};

/*
===============================================================================

FILE IO

===============================================================================
*/

IFile *FS_FileOpenRead(const char *path, int *hndl)
{
	return 0; // TODO
};

IFile *FS_FileOpenWrite(const char *path)
{
	return gpFileSystem->FileOpen(path, "wb");
};

void FS_FileClose(IFile *handle)
{
	//handle->Close();
	gpFileSystem->FileClose(handle);
};

void FS_FileSeek(IFile *handle, int position)
{
	handle->Seek(position);
};

int FS_FileRead(IFile *handle, void *dest, int count)
{
	return handle->Read(dest, count);
};

int FS_FileWrite(IFile *handle, const void *data, int count)
{
	return handle->Write(data, count);
};

int FS_FileTime(const char *path)
{
	return gpFileSystem->GetFileTime(path);
};

void FS_mkdir(const char *path)
{
	// TODO: if not dedicated?

#ifdef _WIN32
	//_mkdir(path); // TODO
#else
	mkdir(path, 0777);
#endif
};