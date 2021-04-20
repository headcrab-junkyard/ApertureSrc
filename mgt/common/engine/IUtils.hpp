/// @file

#pragma once

#include "CommonTypes.hpp"
#include "tier1/interface.h"

constexpr auto OGS_UTILS_INTERFACE_VERSION{"OGSUtils0001"};

interface IUtils : public IBaseInterface
{
	virtual short LittleShort(short l) = 0;
	
	virtual byte *COM_LoadStackFile(const char *path, void *buffer, int bufsize) = 0;
};