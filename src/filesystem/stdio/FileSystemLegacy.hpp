/*
 * This file is part of OGSNext Engine
 *
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

//#include <cstdio>

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

	void AddSearchPath(const char *asPath, const char *asPathID) override;
	bool RemoveSearchPath(const char *asPath) override;

	void RemoveFile(const char *asRelativePath, const char *asPathID) override;

	void CreateDirHierarchy(const char *asPath, const char *asPathID) override;

	bool FileExists(const char *asFileName) override;
	bool IsDirectory(const char *asFileName) override;

	FileHandle_t Open(const char *asFileName, const char *asOptions, const char *asPathID) override;
	void Close(FileHandle_t anHandle) override;

	void Seek(FileHandle_t anHandle, int anPos, FileSystemSeek_t aeSeekType) override;
	unsigned int Tell(FileHandle_t anHandle) override;

	unsigned int Size(FileHandle_t anHandle) override;
	unsigned int Size(const char *asFileName) override;

	long GetFileTime(const char *asFileName) override;
	void FileTimeToString(char *pStrip, int maxCharsIncludingTerminator, long fileTime) override;

	bool IsOk(FileHandle_t anHandle) override;

	void Flush(FileHandle_t anHandle) override;
	bool EndOfFile(FileHandle_t anHandle) override;

	int Read(void *pOutput, int size, FileHandle_t anHandle) override;
	int Write(void const *pInput, int size, FileHandle_t anHandle) override; // void const
	char *ReadLine(char *pOutput, int maxChars, FileHandle_t anHandle) override;
	int FPrintf(FileHandle_t anHandle, char *pFormat, ...) override;

	void *GetReadBuffer(FileHandle_t anHandle, int *outBufferSize, bool failIfNotInCache) override;
	void ReleaseReadBuffer(FileHandle_t anHandle, void *readBuffer) override;

	const char *FindFirst(const char *sWildCard, FileFindHandle_t *pHandle, const char *sPathID) override;
	const char *FindNext(FileFindHandle_t anHandle) override;
	
	bool FindIsDirectory(FileFindHandle_t anHandle) override;
	void FindClose(FileFindHandle_t anHandle) override;

	void GetLocalCopy(const char *sFileName) override;

	const char *GetLocalPath(const char *sFileName, char *pLocalPath, int localPathBufferSize) override;

	char *ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted) override;

	bool FullPathToRelativePath(const char *asFullpath, char *pRelative) override;

	bool GetCurrentDirectory(char *asDirectory, int maxlen) override;

	void PrintOpenedFiles() override;

	void SetWarningFunc(void (*pfnWarning)(const char *fmt, ...)) override;
	void SetWarningLevel(FileWarningLevel_t level) override;

	void LogLevelLoadStarted(const char *asName) override;
	void LogLevelLoadFinished(const char *asName) override;
	
	int HintResourceNeed(const char *asHintList, int anForgetEverything) override;
	
	int PauseResourcePreloading() override;
	int ResumeResourcePreloading() override;
	
	int SetVBuf(FileHandle_t stream, char *buffer, int anMode, long anSize) override;
	
	void GetInterfaceVersion(char *p, int anMaxLen) override;
	
	bool IsFileImmediatelyAvailable(const char *asFileName) override;

	WaitForResourcesHandle_t WaitForResources(const char *asResourceList) override;
	bool GetWaitForResourcesProgress(WaitForResourcesHandle_t anHandle,
	                                 float *progress /* out */,
	                                 bool *complete /* out */) override;
	void CancelWaitForResources(WaitForResourcesHandle_t anHandle) override;

	bool IsAppReadyForOfflinePlay(int anAppID) override;

	bool AddPackFile(const char *asFullPath, const char *asPathID) override;

	FileHandle_t OpenFromCacheForRead(const char *asFileName, const char *asOptions, const char *asPathID) override;

	void AddSearchPathNoWrite(const char *asPath, const char *asPathID) override;
private:
	IFile *ToFile(FileHandle_t anFileHandle) const;
};

// TODO: fix
#undef IFileSystem