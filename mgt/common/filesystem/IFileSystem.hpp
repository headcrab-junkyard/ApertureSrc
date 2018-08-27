/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2017-2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief filesystem interface

#pragma once

#include "public/interface.h"

constexpr auto MGT_FILESYSTEM_INTERFACE_VERSION{"MGTFILESYSTEM001"};

//using FileHandle_t = void *;

struct IFile;

struct IFileSystem : public IBaseInterface
{
	///
	virtual void Mount() = 0;
	
	///
	virtual void Unmount() = 0;
	
	///
	virtual void AddSearchPath(const char *path, const char *alias, bool nowrite = false) = 0;
	
	///
	//virtual void RemoveSearchPath(const char *alias) = 0;
	
	///
	virtual void RemoveAllSearchPaths() = 0; // ClearAllSearchPaths
	
	///
	virtual IFile *OpenPathID(const char *asPath, const char *asPathID) = 0;
	
	///
	virtual IFile *OpenFile(const char *asName, const char *asMode) = 0;
	
	///
	virtual void CloseFile(IFile *apFile) = 0;
	
	///
	//virtual IFile *FileOpen(const char *path, const char *mode) = 0;
	
	///
	//virtual void FileClose(IFile *handle) = 0;
	
	///
	virtual void FileSeek(int handle, int position) = 0;
	
	///
	virtual int FileRead(int handle, void *dest, int count) = 0;
	
	///
	virtual int FileWrite(int handle, const void *data, int count) = 0;
	
	///
	virtual int GetFileTime(const char *path) const = 0;
	
	///
	virtual int GetFileSize(const char *path) const = 0;
};