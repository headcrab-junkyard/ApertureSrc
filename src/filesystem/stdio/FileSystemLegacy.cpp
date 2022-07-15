/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2018, 2020-2022 BlackPhrase
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

#include <cstdio>
#include <cstdarg>
#include <cstring>

#include "FileSystemLegacy.hpp"
#include "FileSystem.hpp"

#include <filesystem/IFile.hpp>

#ifdef _DEBUG
static FILE *ghLogFile = nullptr;

void TRACE_LOG(const char *asMsg, ...)
{
	if(!ghLogFile)
		return;

	va_list ArgList;
	char sMsg[512];

	va_start(ArgList, asMsg);
	vsnprintf(sMsg, sizeof(sMsg), asMsg, ArgList);
	va_end(ArgList);

	sprintf(sMsg, "%s\n", sMsg);
	fputs(sMsg, ghLogFile);
	fflush(ghLogFile);
};
#else
void TRACE_LOG(const char *asMsg, ...){};
#endif

extern CFileSystem *gpFileSystem;

//static CFileSystemLegacy gFileSystemLegacy;
//EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION, gFileSystem);
EXPOSE_SINGLE_INTERFACE(CFileSystemLegacy, IFileSystem009, FILESYSTEM_INTERFACE_VERSION);

CFileSystemLegacy::CFileSystemLegacy() = default;
CFileSystemLegacy::~CFileSystemLegacy() = default;

void CFileSystemLegacy::Mount()
{
#ifdef _DEBUG
	ghLogFile = fopen("TRACE.log", "w+");
#endif
	
	gpFileSystem->Mount();
	
	TRACE_LOG("CFileSystem::Mount");
};

void CFileSystemLegacy::Unmount()
{
	gpFileSystem->Unmount();
	
	TRACE_LOG("CFileSystem::Unmount");

#ifdef _DEBUG
	if(ghLogFile)
		fclose(ghLogFile);
#endif
};

void CFileSystemLegacy::RemoveAllSearchPaths()
{
	TRACE_LOG("CFileSystem::RemoveAllSearchPaths");
	
	gpFileSystem->RemoveAllSearchPaths();
};

void CFileSystemLegacy::AddSearchPath(const char *asPath, const char *asPathID)
{
	TRACE_LOG("CFileSystem::AddSearchPath(%s, %s)", asPath, asPathID);
	gpFileSystem->AddSearchPath(asPath, asPathID, false);
};

bool CFileSystemLegacy::RemoveSearchPath(const char *asPath)
{
	TRACE_LOG("CFileSystem::RemoveSearchPath(%s)", asPath);
	return gpFileSystem->RemoveSearchPath(asPath);
};

void CFileSystemLegacy::RemoveFile(const char *asRelativePath, const char *asPathID)
{
	TRACE_LOG("CFileSystem::RemoveFile(%s, %s)", asRelativePath, asPathID);
	gpFileSystem->RemoveFile(asRelativePath, asPathID);
};

void CFileSystemLegacy::CreateDirHierarchy(const char *asPath, const char *asPathID)
{
	TRACE_LOG("CFileSystem::CreateDirHierarchy(%s, %s)", asPath, asPathID);
	// TODO
	//gpFileSystem->CreateDirHierarchy(asPath, asPathID);
};

bool CFileSystemLegacy::FileExists(const char *asFileName)
{
	TRACE_LOG("CFileSystem::FileExists(%s)", asFileName);
	return gpFileSystem->IsFileExists(asFileName);
};

bool CFileSystemLegacy::IsDirectory(const char *asFileName)
{
	TRACE_LOG("CFileSystem::IsDirectory(%s)", asFileName);
	return gpFileSystem->IsDirectory(asFileName);
};

FileHandle_t CFileSystemLegacy::Open(const char *asFileName, const char *asOptions, const char *asPathID)
{
	TRACE_LOG("CFileSystem::Open(%s, %s, %s)", asFileName, asOptions, asPathID);
	// TODO
	//return reinterpret_cast<FileHandle_t>(gpFileSystem->OpenFile(asFileName, asOptions, asPathID));
	return 0;
};

void CFileSystemLegacy::Close(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::Close(%d)", anHandle);
	gpFileSystem->CloseFile(ToFile(anHandle));
};

void CFileSystemLegacy::Seek(FileHandle_t anHandle, int anPos, FileSystemSeek_t aeSeekType)
{
	TRACE_LOG("CFileSystem::Seek(%d, %d, %d)", anHandle, anPos, aeSeekType);
	ToFile(anHandle)->Seek(anPos, static_cast<IFile::SeekMode>(aeSeekType));
};

unsigned int CFileSystemLegacy::Tell(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::Tell(%d)", anHandle);
	return ToFile(anHandle)->Tell();
};

unsigned int CFileSystemLegacy::Size(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::Size(%d)", anHandle);
	return ToFile(anHandle)->GetSize();
};

unsigned int CFileSystemLegacy::Size(const char *asFileName)
{
	TRACE_LOG("CFileSystem::Size(%s)", asFileName);
	return gpFileSystem->GetFileSize(asFileName);
};

long CFileSystemLegacy::GetFileTime(const char *asFileName)
{
	TRACE_LOG("CFileSystem::GetFileTime(%s)", asFileName);
	return gpFileSystem->GetFileTime(asFileName);
};

void CFileSystemLegacy::FileTimeToString(char *asStrip, int anMaxCharsIncludingTerminator, long anFileTime)
{
	TRACE_LOG("CFileSystem::FileTimeToString(%s, %d, %l", asStrip, anMaxCharsIncludingTerminator, anFileTime);
	// TODO
	//gpFileSystem->FileTimeToString(asStrip, anMaxCharsIncludingTerminator, anFileTime);
	// TODO: gpFileSystem->GetFileTime(...)->ToString()?
};

bool CFileSystemLegacy::IsOk(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::IsOk(%d)", anHandle);
	return ToFile(anHandle)->IsOk();
};

void CFileSystemLegacy::Flush(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::Flush(%d)", anHandle);
	ToFile(anHandle)->Flush();
};

bool CFileSystemLegacy::EndOfFile(FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::EndOfFile(%d)", anHandle);
	return ToFile(anHandle)->IsEOF();
};

int CFileSystemLegacy::Read(void *apOutput, int anSize, FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::Read(%p, %d, %d)", apOutput, anSize, anHandle);
	return ToFile(anHandle)->Read(apOutput, anSize);
};

int CFileSystemLegacy::Write(const void *apInput, int anSize, FileHandle_t anHandle) // void const
{
	TRACE_LOG("CFileSystem::Write(%p, %d, %d)", apInput, anSize, anHandle);
	return ToFile(anHandle)->Write(apInput, anSize);
};

char *CFileSystemLegacy::ReadLine(char *asOutput, int anMaxChars, FileHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::ReadLine(%s, %d, %d)", asOutput, anMaxChars, anHandle);
	// TODO
	//return ToFile(anHandle)->ReadLine(asOutput, anMaxChars);
	return nullptr;
};

int CFileSystemLegacy::FPrintf(FileHandle_t anHandle, char *asFormat, ...)
{
	// TODO
	TRACE_LOG("CFileSystem::FPrintf(%d, %s)", anHandle, asFormat);
	return ToFile(anHandle)->Printf(asFormat);
};

void *CFileSystemLegacy::GetReadBuffer(FileHandle_t anHandle, int *apOutBufferSize, bool abFailIfNotInCache)
{
	TRACE_LOG("CFileSystem::GetReadBuffer(%d, %p, %s)", anHandle, apOutBufferSize, abFailIfNotInCache ? "true" : "false");
	return ToFile(anHandle)->GetReadBuffer(anOutBufferSize, abFailIfNotInCache);
};

void CFileSystemLegacy::ReleaseReadBuffer(FileHandle_t anHandle, void *apBuffer)
{
	TRACE_LOG("CFileSystem::ReleaseReadBuffer(%d, %p)", anHandle, apBuffer);
	ToFile(anHandle)->ReleaseReadBuffer(apBuffer);
};

const char *CFileSystemLegacy::FindFirst(const char *asWildCard, FileFindHandle_t *apHandle, const char *asPathID)
{
	TRACE_LOG("CFileSystem::FindFirst(%s, %p, %d)", asWildCard, apHandle, asPathID);
	// TODO
	//return gpFileSystem->FindFirst(asWildCard, apHandle, asPathID);
	return "";
};

const char *CFileSystemLegacy::FindNext(FileFindHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::FindNext(%d)", anHandle);
	// TODO
	//return gpFileSystem->FindNext(anHandle);
	return "";
};

bool CFileSystemLegacy::FindIsDirectory(FileFindHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::FindIsDirectory(%d)", anHandle);
	// TODO
	//return gpFileSystem->FindIsDirectory(anHandle);
	return false;
};

void CFileSystemLegacy::FindClose(FileFindHandle_t anHandle)
{
	TRACE_LOG("CFileSystem::FindClose(%d)", anHandle);
	// TODO
	//gpFileSystem->FindClose(anHandle);
};

void CFileSystemLegacy::GetLocalCopy(const char *asFileName)
{
	TRACE_LOG("CFileSystem::GetLocalCopy(%s)", asFileName);
	// TODO
	//gpFileSystem->GetLocalCopy(asFileName);
};

const char *CFileSystemLegacy::GetLocalPath(const char *asFileName, char *asLocalPath, int anBufferSize)
{
	TRACE_LOG("CFileSystem::GetLocalPath(%s, %s, %d)", asFileName, asLocalPath, anBufferSize);
	// TODO
	//return gpFileSystem->GetLocalPath(asFileName, asLocalPath, anBufferSize);
	return "";
};

char *CFileSystemLegacy::ParseFile(char *apFileBytes, char *apToken, bool *apWasQuoted)
{
	TRACE_LOG("CFileSystem::ParseFile(%s, %s, %p", apFileBytes, apToken, apWasQuoted);
	// TODO
	//return gpFileSystem->ParseFile(apFileBytes, apToken, apWasQuoteds);
	return nullptr;
};

bool CFileSystemLegacy::FullPathToRelativePath(const char *asFullPath, char *asRelativePath)
{
	TRACE_LOG("CFileSystem::FullPathToRelativePath(%s, %s)", asFullPath, asRelativePath);
	// TODO
	//return gpFileSystem->FullPathToRelativePath(asFullPath, asRelativePath);
	return false;
};

bool CFileSystemLegacy::GetCurrentDirectory(char *asDirectory, int anMaxLen)
{
	TRACE_LOG("CFileSystem::GetCurrentDirectory(%s, %d)", asDirectory, anMaxLen);
	// TODO
	//return gpFileSystem->GetCurrentDirectory(asDirectory, anMaxLen);
	return false;
};

void CFileSystemLegacy::PrintOpenedFiles()
{
	TRACE_LOG("CFileSystem::PrintOpenedFiles()");
	// TODO
	//gpFileSystem->PrintOpenedFiles();
};

void CFileSystemLegacy::SetWarningFunc(void (*pfnWarning)(const char *fmt, ...))
{
	TRACE_LOG("CFileSystem::SetWarningFunc");
	// TODO
	//gpFileSystem->SetWarningFunc(pfnWarning);
};

void CFileSystemLegacy::SetWarningLevel(FileWarningLevel_t aeLevel)
{
	TRACE_LOG("CFileSystem::SetWarningLevel(%d)", aeLevel);
	gpFileSystem->SetWarningLevel(aeLevel);
};

void CFileSystemLegacy::LogLevelLoadStarted(const char *asName)
{
	TRACE_LOG("CFileSystem::LogLevelLoadStarted(%s)", asName);
	// TODO
	//gpFileSystem->LogLevelLoadStarted(asName);
};

void CFileSystemLegacy::LogLevelLoadFinished(const char *asName)
{
	TRACE_LOG("CFileSystem::LogLevelLoadFinished(%s)", asName);
	// TODO
	//gpFileSystem->LogLevelLoadFinished(asName);
};

int CFileSystemLegacy::HintResourceNeed(const char *asHintList, int anForgetEverything)
{
	TRACE_LOG("CFileSystem::HintResourceNeed(%s, %d)", asHintList, anForgetEverything);
	// TODO
	//return gpFileSystem->HintResourceNeed(asHintList, anForgetEverything);
	return 0;
};

int CFileSystemLegacy::PauseResourcePreloading()
{
	TRACE_LOG("CFileSystem::PauseResourcePreloading()");
	// TODO
	//return gpFileSystem->PauseResourcePreloading();
	return 0;
};

int CFileSystemLegacy::ResumeResourcePreloading()
{
	TRACE_LOG("CFileSystem::ResumeResourcePreloading()");
	// TODO
	//return gpFileSystem->ResumeResourcePreloading();
	return 0;
};

int CFileSystemLegacy::SetVBuf(FileHandle_t anHandle, char *apBuffer, int anMode, long anSize)
{
	TRACE_LOG("CFileSystem::SetVBuf(%d, %s, %d, %l)", anHandle, apBuffer, anMode, anSize);
	return ToFile(anHandle)->SetVBuf(apBuffer, anMode, anSize);
};

void CFileSystemLegacy::GetInterfaceVersion(char *p, int maxlen)
{
	TRACE_LOG("CFileSystem::GetInterfaceVersion(%s, %d)", p, maxlen);
	// TODO
	//gpFileSystem->GetInterfaceVersion(p, maxlen);
};

bool CFileSystemLegacy::IsFileImmediatelyAvailable(const char *asFileName)
{
	TRACE_LOG("CFileSystem::IsFileImmediatelyAvailable(%s)", asFileName);
	// TODO
	//return gpFileSystem->IsFileImmediatelyAvailable(asFileName);
	return false;
};

WaitForResourcesHandle_t CFileSystemLegacy::WaitForResources(const char *asResourceList)
{
	TRACE_LOG("CFileSystem::WaitForResources(%s)", asResourceList);
	
	// TODO
	//return gpFileSystem->WaitForResources(asResourceList);
	return 0;
};

bool CFileSystemLegacy::GetWaitForResourcesProgress(WaitForResourcesHandle_t handle,
								 float *progress /* out */,
								 bool *complete /* out */)
{
	// TODO
	TRACE_LOG("CFileSystem::GetWaitForResourcesProgress()", handle, progress, complete);
	// TODO
	//return gpFileSystem->GetWaitForResourcesProgress(progress, complete);
	return false;
};

void CFileSystemLegacy::CancelWaitForResources(WaitForResourcesHandle_t handle)
{
	TRACE_LOG("CFileSystem::CancelWaitForResources(%d)", handle);
	
	// TODO
	//gpFileSystem->CancelWaitForResources(handle);
};

bool CFileSystemLegacy::IsAppReadyForOfflinePlay(int anAppID)
{
	TRACE_LOG("CFileSystem::IsAppReadyForOfflinePlay(%d)", anAppID);
	// TODO
	//return gpFileSystem->IsAppReadyForOfflinePlay(anAppID);
	return false;
};

bool CFileSystemLegacy::AddPackFile(const char *asFullPath, const char *asPathID)
{
	TRACE_LOG("CFileSystem::AddPackFile(%s, %s)", asFullPath, asPathID);
	return gpFileSystem->AddPackFile(asFullPath, asPathID);
};

FileHandle_t CFileSystemLegacy::OpenFromCacheForRead(const char *asFileName, const char *asOptions, const char *asPathID)
{
	TRACE_LOG("CFileSystem::OpenFromCacheForRead(%s, %s, %s)", asFileName, asOptions, asPathID);
	// TODO
	//return reinterpret_cast<FileHandle_t>(gpFileSystem->OpenFromCacheForRead(asFileName, asOptions, asPathID));
	return 0;
};

void CFileSystemLegacy::AddSearchPathNoWrite(const char *asPath, const char *asPathID)
{
	TRACE_LOG("CFileSystem::AddSearchPathNoWrite(%s, %s)", asPath, asPathID);
	
	gpFileSystem->AddSearchPath(asPath, asPathID, true);
};

IFile *CFileSystemLegacy::ToFile(FileHandle_t anFileHandle) const
{
	return reinterpret_cast<IFile*>(anFileHandle);
};