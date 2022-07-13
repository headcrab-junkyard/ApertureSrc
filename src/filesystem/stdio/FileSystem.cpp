/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020-2021 BlackPhrase
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

#include "FileSystem.hpp"

namespace fs = std::filesystem;
CFileSystem gFileSystem;
CFileSystem *gpFileSystem{&gFileSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CFileSystem, IFileSystem, OGS_FILESYSTEM_INTERFACE_VERSION, gFileSystem);

CFileSystem::CFileSystem()
{
};

CFileSystem::~CFileSystem()
{
};

void CFileSystem::Mount()
{
};

void CFileSystem::Unmount()
{
};

void CFileSystem::AddSearchPath(const char *asPath, const char *asAlias, bool abReadOnly)
{
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

IFile *CFileSystem::OpenFile(const char *asName, const char *asMode)
{
	return nullptr;
};

void CFileSystem::CloseFile(IFile *apFile)
{
};

void CFileSystem::RemoveFile(const char *asRelativePath, const char *asAlias)
{
	fs::remove(mRootDir/asRelativePath); // TODO: remove_all?
};

int CFileSystem::GetFileTime(const char *asPath) const
{
	return -1;
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

};