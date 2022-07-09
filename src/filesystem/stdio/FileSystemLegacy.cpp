/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020-2022 BlackPhrase
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

#include "FileSystemLegacy.hpp"
#include "FileSystem.hpp"

#include <filesystem/IFile.hpp>

extern CFileSystem *gpFileSystem;

CFileSystemLegacy::CFileSystemLegacy() = default;
CFileSystemLegacy::~CFileSystemLegacy() = default;

void CFileSystemLegacy::Mount()
{
	gpFileSystem->Mount();
};

void CFileSystemLegacy::Unmount()
{
	gpFileSystem->Unmount();
};

void CFileSystemLegacy::RemoveAllSearchPaths()
{
	gpFileSystem->RemoveAllSearchPaths();
};

void CFileSystemLegacy::AddSearchPath(const char *asPath, const char *asPathID)
{
	gpFileSystem->AddSearchPath(asPath, asPathID, false);
};

bool CFileSystemLegacy::RemoveSearchPath(const char *asPath)
{
	return gpFileSystem->RemoveSearchPath(asPath);
};

void CFileSystemLegacy::RemoveFile(const char *asRelativePath, const char *asPathID)
{
	// TODO
	//gpFileSystem->RemoveFile(asRelativePath, asPathID);
};

void CFileSystemLegacy::CreateDirHierarchy(const char *asPath, const char *asPathID)
{
	// TODO
	//gpFileSystem->CreateDirHierarchy(asPath, asPathID);
};

bool CFileSystemLegacy::FileExists(const char *asFileName)
{
	return gpFileSystem->IsFileExists(asFileName);
};

bool CFileSystemLegacy::IsDirectory(const char *asFileName)
{
	return gpFileSystem->IsDirectory(asFileName);
};

FileHandle_t CFileSystemLegacy::Open(const char *asFileName, const char *asOptions, const char *asPathID)
{
	// TODO
	//return reinterpret_cast<FileHandle_t>(gpFileSystem->OpenFile(asFileName, asOptions, asPathID));
	return 0;
};

void CFileSystemLegacy::Close(FileHandle_t anHandle)
{
	gpFileSystem->CloseFile(ToFile(anHandle));
};

void CFileSystemLegacy::Seek(FileHandle_t anHandle, int anPos, FileSystemSeek_t aeSeekType)
{
	ToFile(anHandle)->Seek(anPos, static_cast<IFile::SeekMode>(aeSeekType));
};

unsigned int CFileSystemLegacy::Tell(FileHandle_t anHandle)
{
	return ToFile(anHandle)->Tell();
};

unsigned int CFileSystemLegacy::Size(FileHandle_t anHandle)
{
	return ToFile(anHandle)->GetSize();
};

unsigned int CFileSystemLegacy::Size(const char *asFileName)
{
	return gpFileSystem->GetFileSize(asFileName);
};

long CFileSystemLegacy::GetFileTime(const char *asFileName)
{
	return gpFileSystem->GetFileTime(asFileName);
};

void CFileSystemLegacy::FileTimeToString(char *asStrip, int anMaxCharsIncludingTerminator, long anFileTime)
{
	// TODO
	//gpFileSystem->FileTimeToString(asStrip, anMaxCharsIncludingTerminator, anFileTime);
};

bool CFileSystemLegacy::IsOk(FileHandle_t anHandle)
{
	return ToFile(anHandle)->IsOk();
};

void CFileSystemLegacy::Flush(FileHandle_t anHandle)
{
	ToFile(anHandle)->Flush();
};

bool CFileSystemLegacy::EndOfFile(FileHandle_t anHandle)
{
	return ToFile(anHandle)->IsEOF();
};

int CFileSystemLegacy::Read(void *apOutput, int anSize, FileHandle_t anHandle)
{
	return ToFile(anHandle)->Read(apOutput, anSize);
};

int CFileSystemLegacy::Write(const void *apInput, int anSize, FileHandle_t anHandle) // void const
{
	return ToFile(anHandle)->Write(apInput, anSize);
};

char *CFileSystemLegacy::ReadLine(char *asOutput, int anMaxChars, FileHandle_t anHandle)
{
	// TODO
	//return ToFile(anHandle)->ReadLine(asOutput, anMaxChars);
	return nullptr;
};

int CFileSystemLegacy::FPrintf(FileHandle_t anHandle, char *asFormat, ...)
{
	// TODO
	return ToFile(anHandle)->Printf(asFormat);
};

void *CFileSystemLegacy::GetReadBuffer(FileHandle_t anHandle, int *anOutBufferSize, bool abFailIfNotInCache)
{
	// TODO
	//return ToFile(anHandle)->GetReadBuffer(anOutBufferSize, abFailIfNotInCache);
	return nullptr;
};

void CFileSystemLegacy::ReleaseReadBuffer(FileHandle_t anHandle, void *apBuffer)
{
	// TODO
	//ToFile(anHandle)->ReleaseReadBuffer(apBuffer);
};

const char *CFileSystemLegacy::FindFirst(const char *asWildCard, FileFindHandle_t *apHandle, const char *asPathID)
{
	// TODO
	//return gpFileSystem->FindFirst(asWildCard, apHandle, asPathID);
	return "";
};

const char *CFileSystemLegacy::FindNext(FileFindHandle_t anHandle)
{
	// TODO
	//return gpFileSystem->FindNext(anHandle);
	return "";
};

bool CFileSystemLegacy::FindIsDirectory(FileFindHandle_t anHandle)
{
	// TODO
	//return gpFileSystem->FindIsDirectory(anHandle);
	return false;
};

void CFileSystemLegacy::FindClose(FileFindHandle_t anHandle)
{
	// TODO
	//gpFileSystem->FindClose(anHandle);
};

void CFileSystemLegacy::GetLocalCopy(const char *asFileName)
{
	// TODO
	//gpFileSystem->GetLocalCopy(asFileName);
};

const char *CFileSystemLegacy::GetLocalPath(const char *asFileName, char *asLocalPath, int anBufferSize)
{
	// TODO
	//return gpFileSystem->GetLocalPath(asFileName, asLocalPath, anBufferSize);
	return "";
};

char *CFileSystemLegacy::ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted)
{
	// TODO
	//return gpFileSystem->ParseFile();
	return nullptr;
};

bool CFileSystemLegacy::FullPathToRelativePath(const char *asFullPath, char *asRelativePath)
{
	// TODO
	//return gpFileSystem->FullPathToRelativePath(asFullPath, asRelativePath);
	return false;
};

bool CFileSystemLegacy::GetCurrentDirectory(char *asDirectory, int anMaxLen)
{
	// TODO
	//return gpFileSystem->GetCurrentDirectory(asDirectory, anMaxLen);
	return false;
};

void CFileSystemLegacy::PrintOpenedFiles()
{
	// TODO
	//gpFileSystem->PrintOpenedFiles();
};

void CFileSystemLegacy::SetWarningFunc(void (*pfnWarning)(const char *fmt, ...))
{
	// TODO
	//gpFileSystem->SetWarningFunc(pfnWarning);
};

void CFileSystemLegacy::SetWarningLevel(FileWarningLevel_t aeLevel)
{
	// TODO
	//gpFileSystem->SetWarningLevel(aeLevel);
};

void CFileSystemLegacy::LogLevelLoadStarted(const char *asName)
{
	// TODO
	//gpFileSystem->LogLevelLoadStarted(asName);
};

void CFileSystemLegacy::LogLevelLoadFinished(const char *asName)
{
	// TODO
	//gpFileSystem->LogLevelLoadFinished(asName);
};

int CFileSystemLegacy::HintResourceNeed(const char *asHintList, int anForgetEverything)
{
	// TODO
	//return gpFileSystem->HintResourceNeed(asHintList, anForgetEverything);
	return 0;
};

int CFileSystemLegacy::PauseResourcePreloading()
{
	// TODO
	//return gpFileSystem->PauseResourcePreloading();
	return 0;
};

int CFileSystemLegacy::ResumeResourcePreloading()
{
	// TODO
	//return gpFileSystem->ResumeResourcePreloading();
	return 0;
};

int CFileSystemLegacy::SetVBuf(FileHandle_t anHandle, char *apBuffer, int anMode, long anSize)
{
	return ToFile(anHandle)->SetVBuf(apBuffer, anMode, anSize);
};

void CFileSystemLegacy::GetInterfaceVersion(char *p, int maxlen)
{
	// TODO
	//gpFileSystem->GetInterfaceVersion(p, maxlen);
};

bool CFileSystemLegacy::IsFileImmediatelyAvailable(const char *sFileName)
{
	// TODO
	//return gpFileSystem->IsFileImmediatelyAvailable(sFileName);
	return false;
};

WaitForResourcesHandle_t CFileSystemLegacy::WaitForResources(const char *sResourceList)
{
	// TODO
	//return gpFileSystem->WaitForResources(sResourceList);
	return 0;
};

bool CFileSystemLegacy::GetWaitForResourcesProgress(WaitForResourcesHandle_t handle,
								 float *progress /* out */,
								 bool *complete /* out */)
{
	// TODO
	//return gpFileSystem->GetWaitForResourcesProgress(progress, complete);
	return false;
};

void CFileSystemLegacy::CancelWaitForResources(WaitForResourcesHandle_t handle)
{
	// TODO
	//gpFileSystem->CancelWaitForResources(handle);
};

bool CFileSystemLegacy::IsAppReadyForOfflinePlay(int nAppID)
{
	// TODO
	//return gpFileSystem->IsAppReadyForOfflinePlay(nAppID);
	return false;
};

bool CFileSystemLegacy::AddPackFile(const char *sFullPath, const char *sPathID)
{
	// TODO
	//return gpFileSystem->AddPackFile(sFullPath, sPathID);
	return false;
};

FileHandle_t CFileSystemLegacy::OpenFromCacheForRead(const char *asFileName, const char *asOptions, const char *asPathID)
{
	// TODO
	//return reinterpret_cast<FileHandle_t>(gpFileSystem->OpenFromCacheForRead(asFileName, asOptions, asPathID));
	return 0;
};

void CFileSystemLegacy::AddSearchPathNoWrite(const char *asPath, const char *asPathID)
{
	gpFileSystem->AddSearchPath(asPath, asPathID, true);
};

IFile *CFileSystemLegacy::ToFile(FileHandle_t anFileHandle) const
{
	return reinterpret_cast<IFile*>(anFileHandle);
};