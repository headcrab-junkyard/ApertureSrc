/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2016-2022 BlackPhrase
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

#include <CommonTypes.hpp>

// TODO: fix
#define IFileSystem IFileSystem009

#include <public/FileSystem.h>

interface IFile;

// TODO: CFileSystem009?
class CFileSystemLegacy final : public IFileSystem
{
public:
	CFileSystemLegacy();
	~CFileSystemLegacy();

	void Mount() override;
	void Unmount() override;

	void RemoveAllSearchPaths() override;

	void AddSearchPath(const char *pPath, const char *pathID) override;
	bool RemoveSearchPath(const char *pPath) override;

	void RemoveFile(const char *pRelativePath, const char *pathID) override;

	void CreateDirHierarchy(const char *path, const char *pathID) override;

	bool FileExists(const char *pFileName) override;
	bool IsDirectory(const char *sFileName) override;

	FileHandle_t Open(const char *sFileName, const char *pOptions, const char *pathID) override;
	void Close(FileHandle_t file) override;

	void Seek(FileHandle_t file, int pos, FileSystemSeek_t seekType) override;
	unsigned int Tell(FileHandle_t file) override;

	unsigned int Size(FileHandle_t file) override;
	unsigned int Size(const char *sFileName) override;

	long GetFileTime(const char *sFileName) override;
	void FileTimeToString(char *pStrip, int maxCharsIncludingTerminator, long fileTime) override;

	bool IsOk(FileHandle_t file) override;

	void Flush(FileHandle_t file) override;
	bool EndOfFile(FileHandle_t file) override;

	int Read(void *pOutput, int size, FileHandle_t file) override;
	int Write(void const *pInput, int size, FileHandle_t file) override; // void const
	char *ReadLine(char *pOutput, int maxChars, FileHandle_t file) override;
	int FPrintf(FileHandle_t file, char *pFormat, ...) override;

	void *GetReadBuffer(FileHandle_t file, int *outBufferSize, bool failIfNotInCache) override;
	void ReleaseReadBuffer(FileHandle_t file, void *readBuffer) override;

	const char *FindFirst(const char *sWildCard, FileFindHandle_t *pHandle, const char *sPathID) override;
	const char *FindNext(FileFindHandle_t handle) override;
	
	bool FindIsDirectory(FileFindHandle_t handle) override;
	void FindClose(FileFindHandle_t handle) override;

	void GetLocalCopy(const char *sFileName) override;

	const char *GetLocalPath(const char *sFileName, char *pLocalPath, int localPathBufferSize) override;

	char *ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted) override;

	bool FullPathToRelativePath(const char *sFullpath, char *pRelative) override;

	bool GetCurrentDirectory(char *sDirectory, int maxlen) override;

	void PrintOpenedFiles() override;

	void SetWarningFunc(void (*pfnWarning)(const char *fmt, ...)) override;
	void SetWarningLevel(FileWarningLevel_t level) override;

	void LogLevelLoadStarted(const char *sName) override;
	void LogLevelLoadFinished(const char *sName) override;
	
	int HintResourceNeed(const char *sHintList, int nForgetEverything) override;
	
	int PauseResourcePreloading() override;
	int ResumeResourcePreloading() override;
	
	int SetVBuf(FileHandle_t stream, char *buffer, int mode, long size) override;
	
	void GetInterfaceVersion(char *p, int maxlen) override;
	
	bool IsFileImmediatelyAvailable(const char *sFileName) override;

	WaitForResourcesHandle_t WaitForResources(const char *sResourceList) override;
	bool GetWaitForResourcesProgress(WaitForResourcesHandle_t handle,
	                                 float *progress /* out */,
	                                 bool *complete /* out */) override;
	void CancelWaitForResources(WaitForResourcesHandle_t handle) override;

	bool IsAppReadyForOfflinePlay(int nAppID) override;

	bool AddPackFile(const char *sFullPath, const char *sPathID) override;

	FileHandle_t OpenFromCacheForRead(const char *sFileName, const char *sOptions, const char *sPathID) override;

	void AddSearchPathNoWrite(const char *sPath, const char *sPathID) override;
private:
	IFile *ToFile(FileHandle_t anFileHandle) const;
};

#undef IFileSystem