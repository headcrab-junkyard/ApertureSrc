/*
 * This file is part of OGSNext Engine
 *
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

#include <cstdio>

#include <unordered_map>

class CFileSystem final : public IFileSystem
{
public:
	void PrintOpenedFiles() override;
	
	//void SetWarningFunc(void (*pfnWarning)(const char *fmt, ...)) override;
	//void SetWarningLevel(FileWarningLevel_t level) override;
	
	void LogLevelLoadStarted(const char *asName) override;
	void LogLevelLoadFinished(const char *asName) override;
	
	//int HintResourceNeed(const char *hintlist, int forgetEverything) override;
	
	int PauseResourcePreloading() override;
	int ResumeResourcePreloading() override;
	
	void AddSearchPath(const char *path, const char *alias, bool bNoWrite) override; // NOTE: alias = pathID
	bool RemoveSearchPath(const char *pPath) override;
	
	void RemoveAllSearchPaths() override;
	
	bool AddPackFile(const char *asFullPath, const char *asPathID) override;
	
	//const char *GetLocalPath(const char *pFileName, char *pLocalPath, int localPathBufferSize) override;
	//bool FullPathToRelativePath(const char *pFullpath, char *pRelative) override;
	
	bool GetCurrentDirectory(char *asDirectory, int anMaxLen) const override;
	
	void GetInterfaceVersion(char *p, int anMaxLen) override;
	
	void GetLocalCopy(const char *asFileName) override;
	
	/// @param anMaxChars - max character including a null terminator
	void FileTimeToString(char *asStrip, int anMaxChars, long anFileTime) override;
	
	IFile *OpenPathID(const char *asPath, const char *asPathID) override;
	
	IFile *OpenFile(const char *asName, const char *asMode/*, const char *pathID*/) override;
	//IFile *OpenFromCacheForRead(const char *pFileName, const char *pOptions, const char *pathID) override;
	
	void CloseFile(const IFile &apFile) override;
	
	/*long*/ int GetFileTime(const char *asPath) const override;
	uint GetFileSize(const char *asFileName) const override;
	
	bool FileExists(const char *asFileName) const override;
	
	void CreateDirHierarchy(const char *asPath, const char *asPathID) override;
	
	void RemoveFile(const char *asRelativePath, const char *asPathID) override;
	
	//char *ParseFile(char *pFileBytes, char *pToken, bool *pWasQuoted) override;
	
	bool IsDirectory(const char *asPath) const override;
	bool IsFileImmediatelyAvailable(const char *asFileName) const override;
	
	bool IsAppReadyForOfflinePlay(int anAppID) const override;
	
	//const char *FindFirst(const char *pWildCard, FileFindHandle_t *pHandle, const char *pathID) override;
	//const char *FindNext(FileFindHandle_t handle) override;
	//bool FindIsDirectory(FileFindHandle_t handle) override;
	//void FindClose(FileFindHandle_t handle) override;
	
	//WaitForResourcesHandle_t WaitForResources(const char *resourcelist) override;
	//bool GetWaitForResourcesProgress(WaitForResourcesHandle_t handle, float *progress /* out */, bool *complete /* out */) override;
	//void CancelWaitForResources(WaitForResourcesHandle_t handle) override;
};