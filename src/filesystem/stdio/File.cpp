/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2020 BlackPhrase
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

#include <cassert>
#include "File.hpp"

CFile::CFile(FILE *apHandle) : msHandle(apHandle)
{
	assert(mpHandle);
	
	msPath = "";
	msName = "";
	msExt = "";
};

CFile::CFile(const char *asName, const char *asMode)
	: msName(asName), mpHandle(fopen(asName, asMode))
{
	assert(mpHandle);
	
	//if(!mpHandle)
		//throw std::runtime_error("Nice joke!");
};

CFile::~CFile()
{
	Close();
};

/*int*/ void CFile::Write(const void *data, int count)
{
	if(!data)
		return;
	
	//if(!mpHandle)
		//return;
	
	//if(mbReadOnly)
		//Sys_Error("Attempted to write to read-only file %d!\n", mpHandle);
	
	if(mpHandle)
		/*return*/ fwrite(data, /*1*/ sizeof(char), count, mpHandle);
};

int CFile::Read(void *dest, int count) const
{
	if(!dest)
		return -1;
	
	if(mpHandle)
		return fread(dest, sizeof(char), count, mpHandle);

	return 0;
};

void CFile::Printf(const char *text, ...)
{
	if(!text || !*text)
		return;
	
	if(mpHandle)
		fputs(text, mpHandle);
};

void CFile::Flush()
{
	fflush(mpHandle);
};

int CFile::Seek(int position) const
{
	if(mpHandle)
		return fseek(mpHandle, position, SEEK_SET);

	return 0;
};

int CFile::Tell() const
{
	if(mpHandle)
		return ftell(mpHandle);

	return 0;
};

bool CFile::IsEOF() const
{
	return feof(mpHandle);
};

const char *CFile::GetPath() const
{
	return msPath;
};

const char *CFile::GetName() const
{
	if(mpHandle)
		return msName;

	return "";
};

const char *CFile::GetExt() const
{
	return msExt;
};

int CFile::GetTime() const
{
	if(mpHandle)
		return 1;
	
	return -1;
};

uint CFile::GetSize() const
{
	if(mpHandle)
		return 0; // TODO
	
	return 0;
};

void CFile::Open(const char *asName, const char *asMode)
{
	Close();
	
	mpHandle = fopen(asName, asMode);
	
	if(IsOk())
	{
		// TODO
		msPath = asName;
		msName = asName;
		msExt = asName;
	};
};

void CFile::Close()
{
	if(!IsOk())
	{
		Flush();
		
		if(fclose(mpHandle) == 0)
		{
			msPath = "";
			msName = "";
			msExt = "";
		};
	};
};