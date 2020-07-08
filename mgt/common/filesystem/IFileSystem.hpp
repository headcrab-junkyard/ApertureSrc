/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2020 BlackPhrase
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
/// @brief native filesystem interface

#pragma once

#include "CommonTypes.hpp"
//#include "common/StringList.hpp"
#include "tier1/interface.h"

constexpr auto OGS_FILESYSTEM_INTERFACE_VERSION{"OGSFileSystem002"};

//using FileHandle_t = void *;

interface IFile;

// TODO: IFileSystemNext?
interface IFileSystem : public IBaseInterface
{
	/// Initialize the file system
	virtual void Mount() = 0; // TODO: Init? return bool?
	
	/// Shutdown the file system
	virtual void Unmount(/*bool abClosemfp, bool abReloading*/) = 0; // TODO: Shutdown?
	
	/**
	* Restarts the file system
	*
	* @return true if restarted successfully
	*/
	//virtual bool Restart() const = 0;
	
	/// @return true if the file system is initialized
	//virtual bool IsMounted() const = 0; // TODO: IsInitialized?
	
	///
	virtual void AddSearchPath(const char *asPath, const char *asAlias, bool abReadOnly = false) = 0;
	
	///
	virtual bool RemoveSearchPath(const char *asAlias) = 0;
	
	/// Remove all search paths (including write path)
	virtual void RemoveAllSearchPaths() = 0; // ClearAllSearchPaths
	
	///
	virtual IFile *OpenPathID(const char *asPath, const char *asPathID) = 0;
	
	///
	virtual IFile *OpenFile(const char *asName, const char *asMode) = 0;
	
	/// Closes a file
	virtual void CloseFile(IFile *apFile) = 0;
	
	///
	virtual /*long*/ int GetFileTime(const char *asPath) const = 0;
	
	/// Returns length of file, -1 if no file exists
	virtual uint GetFileSize(const char *asPath) const = 0; // TODO: GetFileLength?
	
	///
	virtual bool IsFileExists(const char *asFileName) const = 0;
	
	///
	virtual bool IsDirectory(const char *asFileName) const = 0;
	
	enum class FolderState : int
	{
		Error = -1,
		No    = 0,
		Yes
	};
	
	/// Renames a file, taken from idTech5 (minus the fsPath_t)
	//virtual bool RenameFile(const char *relativePath, const char *newName, const char *basePath = "fs_savepath") = 0;
	//virtual bool FileRename(const char *asOldName, const char *asNewName) const = 0;
	
	/**
	* Remove the specified file
	*/
	//virtual bool DeleteFile(const char *asRelPath) const = 0;
	
	/// Modes for OpenFileByMode
	enum class FileOpenMode : int
	{
		Read = 0,
		Write,
		Append
	};
	
	/// Opens a file for reading, writing, or appending depending on the value of mode
	//virtual IFile *OpenFile(const char *asRelPath, FileOpenMode aeMode) = 0;
	
	///
	//virtual byte *LoadFile(char *asFileName, int *anLength) const = 0;
	
	/// Frees the memory allocated by ReadFile
	//virtual void FreeFile(void *apBuffer) = 0;
	
	/// Properly handles partial reads
	//int FileRead();
	//virtual void ReadFile(const char *filename, void *buffer, int len) = 0;
	
	///
	//virtual void WriteFile(const char *filename, void *data, int len) = 0;
	
	/** Parse a token from a file */
	//virtual const char *ParseFile(const char *data, char *token, int maxlen) const = 0;
	
	/** Copies a file */
	//virtual bool CopyFile(const char *asSource, const char *asDestination) const = 0;
	
	/** Used by AI node graph code to determine if .bsp and .ain files are out of date */
	//virtual int CompareFileTime(const char *asFileName1, const char *asFileName2, int *anCompare) = 0;
	
	/**
	* Returns FOLDER_YES if the specified path is a folder
	*/
	//virtual /*bool*/ FolderState IsFolder(const char *asPath) const = 0;
	
	/**
	*/
	//virtual void CreatePath(const char *asPath) = 0; // TODO: CreateFolder?
	
	/** Removes the specified directory */
	//virtual bool RemoveFolder(const char *asRelPath) = 0;
	
	/**
	*/
	//virtual bool IsFileWriteable(const char *asPath) const = 0;
	
	// use fs_debug to verbose Sys_ListFiles
	// returns -1 if directory was not found (the list is cleared)
	//virtual int Sys_ListFiles(const char *directory, const char *extension, idList<class idStr> & list) = 0;
	//virtual void GetFilesInDirectory(const char *asPath, const char *asExt, TStringList &apOutList) = 0;
	
	//virtual const char *GetExePath() const = 0;
	//virtual const char *GetCurrentDir() const = 0;
	//virtual const char *GetLaunchPath() const = 0;
};