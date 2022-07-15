/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2016-2020, 2022 BlackPhrase
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

constexpr auto MAX_OSPATH{128}; ///< Max length of a filesystem pathname

#ifndef MAX_QPATH
constexpr auto MAX_QPATH{64};
#endif

#ifndef MAX_OSPATH
constexpr auto MAX_OSPATH{256};
#endif

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
	
	bool IsMounted() const /*override*/ {return mbMounted;}
	
	void SetWarningLevel(FileWarningLevel_t aeLevel) override {meWarningLevel = aeLevel;}
	
	void AddSearchPath(const char *asPath, const char *asAlias, bool abReadOnly) override;
	bool RemoveSearchPath(const char *asAlias) override;
	
	void RemoveAllSearchPaths() override;
	
	bool AddPackFile(const char *asFullPath, const char *asPathID) override;
	
	IFile *OpenPathID(const char *asPath, const char *asPathID) override;
	IFile *OpenFile(const char *asName, const char *asMode) override;
	
	void CloseFile(IFile *apFile) override;
	
	void RemoveFile(const char *asRelativePath, const char *asAlias) override;
	
	/*long*/ int GetFileTime(const char *asFileName) const override;
	uint GetFileSize(const char *asFileName) const override;
	
	bool IsFileExists(const char *asFileName) const override;
	bool IsDirectory(const char *asPath) const override;
private:
	IFile *FindFile(const char *asFileName) const;
	
	pack_t *LoadPackFile(const char *asName);
	void AddGameDirectory(const char *asDir);
	
	int findhandle() const;
	int filelength(FILE *apFile);
private:	
	static constexpr auto MAX_HANDLES{10};
	
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

	std::filesystem::path mRootDir{"."};
	
	//
	// in memory
	//

	struct packfile_t
	{
		char name[MAX_QPATH]{};
		int filepos{0}, filelen{0};
	};

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
	
	FileWarningLevel_t meWarningLevel{0};
	
	char com_cachedir[MAX_OSPATH]{};
	
	bool mbMounted{false};
};