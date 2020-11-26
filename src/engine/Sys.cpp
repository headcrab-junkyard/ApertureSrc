#include "quakedef.h"
#include "Sys.hpp"

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
	Sys_Printf(false, fmt);
};

void CSys::Quit()
{
	Sys_Quit();
};

double CSys::GetDoubleTime()
{
	return Sys_FloatTime(); //Sys_GetDoubleTime();
};

char *CSys::GetConsoleInput()
{
	return Sys_ConsoleInput();
};

/// called to yield for a little bit so as
/// not to hog cpu when paused or debugging
void CSys::Sleep(int anMilliSecs)
{
	Sys_Sleep(anMilliSecs);
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
	//Sys_SetFPCW(); // TODO
};