/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2017-2020 BlackPhrase
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
/// @brief file handle wrapper interface

#pragma once

#include "CommonTypes.hpp"

interface IFile
{
	///
	//virtual bool Open(const char *asName, const char *asOptions) = 0; // TODO: Reopen?
	//virtual void Close() = 0;
	//virtual bool IsOpen() const = 0;
	
	/// Write data from the buffer to the file
	virtual /*int*/ void Write(const void *data, int count) = 0;
	//virtual int Write(const void *apInput, int anSize, int anCount) = 0; // const?
	
	/// Read data from the file to the buffer
	virtual int Read(void *dest, int count) const = 0;
	//virtual int Read(void *apOutput, int anSize, int anCount) = 0; // const?
	
	///
	//virtual char *ReadLine(char *asOutput, int anMaxChars) = 0; // const?
	
	/// Like fprintf
	virtual /*int*/ void Printf(/*VERIFY_FORMAT_STRING*/ const char *text, ...) = 0;
	
	/// Like fprintf but with argument pointer
	//virtual int VPrintf(const char *asMsg, va_list alstArgs) = 0; // TODO: Is it ABI-safe?
	
	enum class SeekMode : int
	{
		Current = 0,
		End,
		Set
	};
	
	/// Seek on a file
	virtual int Seek(int position) const = 0;
	//virtual int Seek(long anOffset, SeekMode aeMode) = 0;
	//virtual void Seek(int anPos, FileSystemSeek_t seekType) = 0;
	//virtual int Seek(long offset, fsOrigin_t origin) = 0;
	
	/// @return offset in file
	virtual /*unsigned*/ int Tell() const = 0;
	
	/// Go back to the beginning of the file
	//virtual void Rewind() = 0;
	
	/// Causes any buffered data to be written to the file
	/// @param abForce - Forces flush on files being writting to
	virtual void Flush(/*bool abForce*/) = 0;
	
	///
	//virtual int IsOk() const = 0;
	
	///
	virtual /*int*/ bool IsEOF() const = 0;
	
	/// @return name of the file
	virtual const char *GetName() const = 0;
	
	/// @return extension of the file (if present)
	virtual const char *GetExt() const = 0;
	
	/// Get the full file path
	virtual const char *GetPath() const = 0; // TODO: GetFullPath?
	
	///
	virtual int GetTime() const = 0;
	
	/// @return the length of the file
	virtual /*unsigned*/ int GetSize() const = 0; // TODO: int GetLength?
	
	/// Return a time value for reload operations
	//virtual ID_TIME_T GetTimeStamp() const = 0;
	
	///
	//virtual byte GetCharacter() const = 0;
	
	//virtual FileHandle_t GetHandle() const = 0;
	//operator bool(){return GetHandle() != nullptr;}
};