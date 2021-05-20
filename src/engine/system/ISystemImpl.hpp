/// @file

#pragma once

#include "CommonTypes.hpp"

interface ISystemImpl
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Printf(const char *asMsg, ...) = 0;
	
	///
	virtual void Error(const char *asMsg) = 0;
	
	///
	virtual double GetFloatTime() = 0;
};