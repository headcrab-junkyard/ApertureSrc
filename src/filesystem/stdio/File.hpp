/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2021 BlackPhrase
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
/// @brief IFile interface implementation

#pragma once

#include <cstdio>

#include "CommonTypes.hpp"
#include "filesystem/IFile.hpp"

class CFile /*final*/ : public IFile
{
public:
	CFile(FILE *apHandle);
	CFile(const char *asPath, const char *asMode);
	~CFile();
	
	// IFile interface methods implementation
	
	size_t Write(const void *apData, size_t anCount) override;
	size_t Read(void *apDest, size_t anCount) const override;

	//char *ReadLine(char *asOutput, int anMaxChars) override;
	
	int Printf(const char *asText, ...) override;
	
	int Seek(long anOffset, SeekMode aeMode) const override;
	
	/*uint*/ long Tell() const override;
	
	void Flush() override;
	
	int SetVBuf(char *apBuffer, int anMode, size_t anSize) override;
	
	//void *GetReadBuffer(int *outBufferSize, bool abFailIfNotInCache) override;
	//void ReleaseReadBuffer(void *apReadBuffer) override;
	
	bool IsOk() const override {return mpHandle != nullptr;} // TODO: IsValid?
	bool IsEOF() const override;
	
	const char *GetPath() const override {return msPath.c_str();} // TODO: GetRelPath?
	const char *GetName() const override; // TODO: GetFileName?
	const char *GetExt() const override {return msExt.c_str();}

	int GetTime() const; //override;
	/*u*/ int GetSize() const override;
public:
	// Other public methods
	
	void Open(const char *asPath, const char *asMode);
	void Close();
	
	/// Like fprintf but with argument pointer
	int VPrintf(const char *asMsg, va_list alstArgs);
private:
	int SeekModeToInt(SeekMode aeMode) const;
private:
	tString msPath{""};
	tString msName{""};
	tString msExt{""};
	
	FILE *mpHandle{nullptr};
};