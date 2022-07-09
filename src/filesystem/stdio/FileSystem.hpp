/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2020, 2022 BlackPhrase
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
/// @brief IFileSystem interface implementation

#pragma once

#include <list>
#include <string>

#include <filesystem/IFileSystem.hpp>

// directory searching
#define SFF_ARCH    0x01
#define SFF_HIDDEN  0x02
#define SFF_RDONLY  0x04
#define SFF_SUBDIR  0x08
#define SFF_SYSTEM  0x10

#define MAX_OSPATH 128 // max length of a filesystem pathname

using pack_t = struct pack_s;

/*
** pass in an attribute mask of things you wish to REJECT
*/
char *Sys_FindFirst(const char *path, unsigned int musthave, unsigned int canthave);
char *Sys_FindNext(unsigned int musthave, unsigned int canthave);
void Sys_FindClose();

struct SSearchPathEntry
{
	std::string name{""};
	
	bool readonly{true}; // TODO: nowrite
};

using tSearchPathEntryList = std::list<SSearchPathEntry>;

struct SSearchPathGroup
{
	std::string name{""}; // TODO: pathid
	tSearchPathEntryList mlstSearchPaths;
};

using tSearchPathGroupList = std::list<SSearchPathGroup>;

class CFileSystem final : public IFileSystem
{
public:
	CFileSystem();
	~CFileSystem();
	
	void Mount() override;
	void Unmount() override;
	
	void AddSearchPath(const char *asPath, const char *asAlias, bool abReadOnly) override;
	bool RemoveSearchPath(const char *asAlias) override;
	
	void RemoveAllSearchPaths() override;
	
	IFile *OpenPathID(const char *asPath, const char *asPathID) override;
	IFile *OpenFile(const char *asName, const char *asMode) override;
	
	void CloseFile(IFile *apFile) override;
	
	int GetFileTime(const char *asPath) const override;
	uint GetFileSize(const char *asPath) const override;
	
	bool IsFileExists(const char *asFileName) const override;
	bool IsDirectory(const char *asFileName) const override;
private:
	static constexpr auto MAX_HANDLES{10};
	
	IFile *FindFile(const char *filename) const;
	
	pack_t *LoadPackFile(const char *packfile);
	void AddGameDirectory(const char *dir);
	
	int findhandle() const;
	int filelength(FILE *f);
	
	tSearchPathGroupList mlstSearchPaths;

#ifdef _WIN32
	// TODO: if WIN only?
	FILE *sys_handles[MAX_HANDLES]{nullptr};
#elif sun
	struct MEMFILE
	{
		FILE *hFile{nullptr};
		char *pMap{nullptr};
		int nLen{0};
		int nPos{0};
	};

	MEMFILE sys_handles[MAX_HANDLES]{};
#endif
};