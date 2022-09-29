/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019-2020, 2022 BlackPhrase
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

#include "quakedef.h"
#include "FileSystem.hpp"

IFile *CFileSystem::FileOpenRead(const char *path)
{
	return FS_FileOpenRead(path);
};

IFile *CFileSystem::FileOpenWrite(const char *path)
{
	return FS_FileOpenWrite(path);
};

void CFileSystem::FileClose(IFile *handle)
{
	FS_FileClose(handle);
};

void CFileSystem::FileSeek(IFile *handle, int position)
{
	FS_FileSeek(handle, position);
};

int CFileSystem::FileRead(IFile *handle, void *dest, int count)
{
	return FS_FileRead(handle, dest, count);
};

int CFileSystem::FileWrite(IFile *handle, void *data, int count)
{
	return FS_FileWrite(handle, data, count);
};

int	CFileSystem::FileTime(const char *path)
{
	return FS_FileTime(path);
};

void CFileSystem::mkdir(const char *path)
{
	FS_mkdir(path);
};