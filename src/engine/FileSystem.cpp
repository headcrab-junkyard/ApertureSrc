#include "quakedef.h"
#include "FileSystem.hpp"

IFile *CFileSystem::FileOpenRead(const char *path)
{
	return FS_FileOpenRead(path);
};

IFile *CFileSystem::FileOpenWrite(const char *path)
{
	return FS_FileOpenWrite(path);
};

void CFileSystem::FileClose(IFile *handle)
{
	FS_FileClose(handle);
};

void CFileSystem::FileSeek(IFile *handle, int position)
{
	FS_FileSeek(handle, position);
};

int CFileSystem::FileRead(IFile *handle, void *dest, int count)
{
	return FS_FileRead(handle, dest, count);
};

int CFileSystem::FileWrite(IFile *handle, void *data, int count)
{
	return FS_FileWrite(handle, data, count);
};

int	CFileSystem::FileTime(const char *path)
{
	return FS_FileTime(path);
};

void CFileSystem::mkdir(const char *path)
{
	FS_mkdir(path);
};