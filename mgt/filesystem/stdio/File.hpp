/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief IFile interface implementation

#pragma once

#include <cstdio>
#include "filesystem/IFile.hpp"

class CFile final : public IFile
{
public:
	CFile(const char *asName, const char *asMode);
	~CFile();
	
	/*int*/ void Write(const void *data, int count) override;
	int Read(void *dest, int count) const override;

	//char *ReadLine(char *pOutput, int maxChars) override;
	
	/*int*/ void Printf(const char *text, ...) override;

	void Flush() override;
	
	/*void*/ //int Seek(int position, FileSystemSeek_t seekType) const; //override; // TODO
	int Seek(int position) const override;
	
	/*unsigned*/ int Tell() const override;

	//int SetVBuf(char *buffer, int mode, long size) override;
	
	//bool IsOK() const override;
	bool IsEOF() const override;
	
	//void *GetReadBuffer(int *outBufferSize, bool failIfNotInCache) override;
	//void ReleaseReadBuffer(void *readBuffer) override;
	
	const char *GetName() const override;
	const char *GetExt() const override;
	const char *GetPath() const override;

	int GetTime() const override;
	/*unsigned*/ int GetSize() const override;
private:
	const char *msName{""};
	
	FILE *mpHandle{nullptr};
};