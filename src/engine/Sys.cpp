#include "quakedef.h"
#include "Sys.hpp"

int CSys::FileOpenRead(const char *path, int *hndl)
{
	return Sys_FileOpenRead(path, hndl);
};

int CSys::FileOpenWrite(const char *path)
{
	return Sys_FileOpenWrite(path);
};

void CSys::FileClose(int handle)
{
	Sys_FileClose(handle);
};

void CSys::FileSeek(int handle, int position)
{
	Sys_FileSeek(handle, position);
};

int CSys::FileRead(int handle, void *dest, int count)
{
	return Sys_FileRead(handle, dest, count);
};

int CSys::FileWrite(int handle, void *data, int count)
{
	return Sys_FileWrite(handle, data, count);
};

int	CSys::FileTime(const char *path)
{
	return Sys_FileTime(path);
};

void CSys::mkdir(const char *path)
{
	Sys_mkdir(path);
};

void CSys::MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
	Sys_MakeCodeWriteable(startaddr, length);
};

void CSys::DebugLog(const char *file, char *fmt, ...)
{
	Sys_DebugLog(file, fmt);
};

void CSys::Error(const char *error, ...)
{
	Sys_Error(error);
};

void CSys::Printf(const char *fmt, ...)
{
	Sys_Printf(fmt);
};

void CSys::Quit()
{
	Sys_Quit();
};

double CSys::GetDoubleTime()
{
	return Sys_GetDoubleTime();
};

char *CSys::GetConsoleInput()
{
	return Sys_ConsoleInput();
};

/// called to yield for a little bit so as
/// not to hog cpu when paused or debugging
void CSys::Sleep()
{
	Sys_Sleep();
};

/// Perform Key_Event() callbacks until the input que is empty
void CSys::SendKeyEvents()
{
	Sys_SendKeyEvents();
};

void CSys::LowFPPrecision()
{
	Sys_LowFPPrecision();
};

void CSys::HighFPPrecision()
{
	Sys_HighFPPrecision();
};

void CSys::SetFPCW()
{
	Sys_SetFPCW();
};