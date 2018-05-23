/// @file

#include "quakedef.h"
#include "System.hpp"

CSystem gSystem;
//CSystem *gpSystem{&gSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSystem, ISystem, MGT_SYSTEM_INTERFACE_VERSION, gSystem);

CSystem::CSystem() = default;
CSystem::~CSystem() = default;

double CSystem::GetFloatTime() const
{
	return Sys_FloatTime();
};