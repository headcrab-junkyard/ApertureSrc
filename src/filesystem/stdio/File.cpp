/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020-2022 BlackPhrase
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
#include <cstdio>

#include "File.hpp"

CFile::CFile(FILE *apHandle) : mpHandle(apHandle)
{
	assert(mpHandle);
	
	msPath = "";
	msName = "";
	msExt = "";
};

CFile::CFile(const char *asName, const char *asMode)
{
	Open(asName, asMode);
	
	assert(mpHandle);
	
	//if(!mpHandle)
		//throw std::runtime_error("Nice joke!");
};

CFile::~CFile()
{
	Close();
};

size_t CFile::Write(const void *data, size_t count)
{
	if(!data)
		return -1;
	
	//if(!mpHandle)
		//return -1;
	
	//int t{VID_ForceUnlockedAndReturnState()}; // TODO
	
	//if(mbReadOnly)
		//Sys_Error("Attempted to write to read-only file %d!\n", mpHandle);
	
	int x;
	
	//int handle = ToIntHandle(mpHandle);
	
#ifdef _WIN32
	//x = fwrite(data, 1, count, sys_handles[handle]);
#else
	//x = write(handle, src, count);
#endif
	
	return fwrite(data, 1 /*sizeof(char)*/, count, mpHandle); // TODO: sizeof(uint8_t)
	
	//VID_ForceLockState(t); // TODO
	//return x;
};

size_t CFile::Read(void *dest, size_t count) const
{
	if(!dest)
		return -1;
	
	//if(!mpHandle)
		//return -1;
	
	//int t{VID_ForceUnlockedAndReturnState()}; // TODO
	
	int x;
	
	//int handle = ToIntHandle(mpHandle);
	
#ifdef _WIN32
	//x = fread(dest, 1, count, sys_handles[handle]);
#else
	//x = read(handle, dest, count);
#endif
	
	return fread(dest, 1 /*sizeof(char)*/, count, mpHandle); // TODO: sizeof(uint8_t)
	
	//VID_ForceLockState(t); // TODO
	//return x;
};

int CFile::Printf(const char *text, ...)
{
	// TODO
	
	if(!text || !*text)
		return -1;
	
	if(mpHandle)
		return fputs(text, mpHandle);
	
	return -1;
};

int CFile::Seek(long position, SeekMode aeMode) const
{
	if(mpHandle)
	{
		int nMode{SeekModeToInt(aeMode)};
		
		//int t{VID_ForceUnlockedAndReturnState()}; // TODO: windows & not dedicated
		
		return fseek(mpHandle, position, nMode);
		
		/*
		int handle = ToIntHandle(mpHandle);
		
#ifdef _WIN32
		fseek(sys_handles[handle], position, SEEK_SET);
#else
		lseek(handle, position, SEEK_SET);
#endif
		*/
		
		//VID_ForceLockState(t); // TODO: windows & not dedicated
	};

	return 0;
};

long CFile::Tell() const
{
	if(mpHandle)
		return ftell(mpHandle);

	return -1L;
};

void CFile::Flush()
{
	fflush(mpHandle);
};

int CFile::SetVBuf(char *apBuffer, int anMode, size_t anSize)
{
	return setvbuf(mpHandle, apBuffer, anMode, anSize);
};

void *CFile::GetReadBuffer(int *apOutBufferSize, bool abFailIfNotInCache) const
{
	// TODO
	return nullptr;
};

void CFile::ReleaseReadBuffer(void *apBuffer)
{
	// TODO
};

bool CFile::IsEOF() const
{
	return feof(mpHandle);
};

const char *CFile::GetName() const
{
	if(mpHandle)
		return msName.c_str();

	return "";
};

int CFile::GetTime() const
{
	if(mpHandle)
		return 1;
	
	return -1;
};

/*u*/ int CFile::GetSize() const
{
	if(mpHandle)
	{
		fseek(mpHandle, 0, SEEK_END);
		return ftell(mpHandle);
	};
	
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
	if(IsOk())
	{
		Flush();
		
		if(fclose(mpHandle) == 0)
		{
			mpHandle = nullptr;
			
			msPath = "";
			msName = "";
			msExt = "";
		};
	};
};

int CFile::SeekModeToInt(SeekMode aeMode) const
{
	switch(aeMode)
	{
	case SeekMode::Set:
		return SEEK_SET;
	case SeekMode::Current:
		return SEEK_CUR;
	case SeekMode::End:
		return SEEK_END;
	};
	
	return -1;
};