/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2020 BlackPhrase
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

#include <cstdio>
#include <string>
#include <list>
#include <unordered_map>
#include "filesystem/IFileSystem.hpp"

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
	
	//void PrintOpenedFiles() override;
	
	//void SetWarningFunc(void (*pfnWarning)(const char *fmt, ...)) override;
	//void SetWarningLevel(FileWarningLevel_t level) override;
	
	//void LogLevelLoadStarted(const char *name) override;
	//void LogLevelLoadFinished(const char *name) override;
	
	//int HintResourceNeed(const char *hintlist, int forgetEverything) override;
	
	//int PauseResourcePreloading() override;
	//int ResumeResourcePreloading() override;
	
	void AddSearchPath(const char *path, const char *alias, bool bNoWrite) override; // NOTE: alias = pathID
	//bool RemoveSearchPath(const char *pPath) override;
	
	void RemoveAllSearchPaths() override;
	
	//bool AddPackFile(const char *fullpath, const char *pathID) override;
	
	//const char *GetLocalPath(const char *pFileName, char *pLocalPath, int localPathBufferSize) override;
	//bool FullPathToRelativePath(const char *pFullpath, char *pRelative) override;
	
	//bool GetCurrentDirectory(char *pDirectory, int maxlen) override;
	
	//void GetInterfaceVersion(char *p, int maxlen) override;
	
	//void GetLocalCopy(const char *pFileName) override;
	
	//void FileTimeToString(char *pStrip, int maxCharsIncludingTerminator, long fileTime) override;
	
	IFile *OpenPathID(const char *asPath, const char *asPathID) override;
	
	IFile *OpenFile(const char *asName, const char *asMode/*, const char *pathID*/) override;
	//IFile *OpenFromCacheForRead(const char *pFileName, const char *pOptions, const char *pathID) override;
	
	void CloseFile(IFile *apFile) override;
	
	//int FileOpen(const char *path, const char *mode) override;
	//void FileClose(int handle) override;
	
	void FileSeek(int handle, int position) override;
	
	int FileRead(int handle, void *dest, int count) override;
	int FileWrite(int handle, const void *data, int count) override;
	
	/*long*/ int GetFileTime(const char *path) const override;
	/*unsigned*/ int GetFileSize(const char *pFileName) const override;
	
	//bool FileExists(const char *pFileName) override;
	
	//void CreateDirHierarchy(const char *path, const char *pathID) override;
	
	//void RemoveFile(const char *pRelativePath, const char *pathID) override;
	
	//char *ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted) override;
	
	//bool IsDirectory(const char *asPath) override;
	//bool IsFileImmediatelyAvailable(const char *pFileName) override;
	
	//bool IsAppReadyForOfflinePlay(int appID) override;
	
	//const char *FindFirst(const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID) override;
	//const char *FindNext(FileFindHandle_t handle) override;
	//bool FindIsDirectory(FileFindHandle_t handle) override;
	//void FindClose(FileFindHandle_t handle) override;
	
	//WaitForResourcesHandle_t WaitForResources(const char *resourcelist) override;
	//bool GetWaitForResourcesProgress(WaitForResourcesHandle_t handle, float *progress /* out */, bool *complete /* out */) override;
	//void CancelWaitForResources(WaitForResourcesHandle_t handle) override;
private:
	static constexpr auto MAX_HANDLES{10};
	
	tSearchPathGroupList mlstSearchPaths;
	
	int findhandle();
	int filelength(FILE *f);

	// TODO: if WIN only?
	FILE *sys_handles[MAX_HANDLES]{nullptr};
};