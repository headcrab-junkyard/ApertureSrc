#include "quakedef.h"
#include "System.hpp"

void CSystem::MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
	Sys_MakeCodeWriteable(startaddr, length);
};

void CSystem::DebugLog(const char *file, char *fmt, ...)
{
	Sys_DebugLog(file, fmt);
};

void CSystem::Error(const char *error, ...)
{
	Sys_Error(error);
};

void CSystem::Printf(const char *fmt, ...)
{
	Sys_Printf(false, fmt);
};

void CSystem::Quit()
{
	Sys_Quit();
};

double CSystem::GetDoubleTime()
{
	return Sys_FloatTime(); //Sys_GetDoubleTime();
};

char *CSystem::GetConsoleInput()
{
	return Sys_ConsoleInput();
};

/// called to yield for a little bit so as
/// not to hog cpu when paused or debugging
void CSystem::Sleep(int anMilliSecs)
{
	Sys_Sleep(anMilliSecs);
};

void CSystem::LowFPPrecision()
{
	Sys_LowFPPrecision();
};

void CSystem::HighFPPrecision()
{
	Sys_HighFPPrecision();
};

void CSystem::SetFPCW()
{
	//Sys_SetFPCW(); // TODO
};