/// @file

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_UTILS_INTERFACE_VERSION{"MGTUtils0001Alpha"};

interface IUtils : public IBaseInterface
{
	virtual short LittleShort(short l) = 0;
	
	virtual byte *COM_LoadStackFile(const char *path, void *buffer, int bufsize) = 0;
};