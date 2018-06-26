/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "System.hpp"
#include "SystemEventDispatcher.hpp"
#include "interface.h"

CSystem gSystem;
//CSystem *gpSystem{&gSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSystem, ISystem, MGT_SYSTEM_INTERFACE_VERSION, gSystem);

CSystem::CSystem()
{
	mpEventDispatcher = std::make_unique<CSystemEventDispatcher>();
};

CSystem::~CSystem() = default;

void CSystem::AddEventListener(ISystemEventListener *apListener)
{
	mpEventDispatcher->AddListener(apListener);
};

void CSystem::RemoveEventListener(ISystemEventListener *apListener)
{
	mpEventDispatcher->RemoveListener(apListener);
};

void CSystem::Error(const char *fmt, ...)
{
	va_list argptr;
	char text[1024]{};

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);

	mpEventDispatcher->DispatchError(text);
	
	Sys_Error(text);
};

double CSystem::GetFloatTime() const
{
	return Sys_FloatTime();
};