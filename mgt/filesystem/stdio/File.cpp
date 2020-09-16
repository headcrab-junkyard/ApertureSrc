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

CFile::CFile(const char *asName, const char *asMode)
	: msName(asName), mpHandle(fopen(asName, asMode))
{
	assert(mpHandle);
	
	//if(!mpHandle)
		//throw std::runtime_error("Nice joke!");
};

CFile::~CFile()
{
	if(mpHandle)
	{
		fflush(mpHandle);
		fclose(mpHandle);
	};
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

const char *CFile::GetName() const
{
	if(mpHandle)
		return msName;

	return "";
};

const char *CFile::GetExt() const
{
	return ""; // TODO
};

const char *CFile::GetPath() const
{
	return ""; // TODO
};

int CFile::GetTime() const
{
	if(mpHandle)
		return 1;
	
	return -1;
};

int CFile::GetSize() const
{
	return 0; // TODO
};