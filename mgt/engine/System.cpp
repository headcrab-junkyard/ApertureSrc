/// @file

#include "quakedef.h"
#include "System.hpp"
#include "interface.h"

CSystem gSystem;
//CSystem *gpSystem{&gSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSystem, ISystem, MGT_SYSTEM_INTERFACE_VERSION, gSystem);

CSystem::CSystem() = default;
CSystem::~CSystem() = default;

void CSystem::Error(const char *fmt, ...)
{
	va_list argptr;
	char text[1024]{};

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);

	Sys_Error(text);
};

double CSystem::GetFloatTime() const
{
	return Sys_FloatTime();
};