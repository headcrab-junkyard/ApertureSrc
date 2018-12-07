/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "System.hpp"
#include "SystemEventDispatcher.hpp"
#include "Interface.hpp"

constexpr auto MAXPRINTMSG{4096};

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

void CSystem::Printf(const char *fmt, ...)
{
	va_list argptr;
	char text[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);
	
	Sys_Printf(false, "%s", text);
};

/*
================
CSystem::DevPrintf

A CSystem::Printf that only shows up if the "developer" cvar is set
================
*/
// FIXME: make a buffer size safe vsprintf?
void CSystem::DevPrintf(const char *fmt, ...)
{
	// don't confuse non-developers with techie stuff...
	if(!developer.GetValue())
		return;
	
	va_list argptr;
	char msg[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);
	
	Printf("%s", msg);
};

void CSystem::Warning(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG]{}; // TODO: 1024 for linux

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);
	
#ifdef _WIN32
	Printf("[WARNING] %s", msg);
#elif __linux__
	fprintf(stderr, "[WARNING] %s", msg); // TODO: u sure?
#endif
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

void CSystem::Quit()
{
	Host_Shutdown();

#ifdef _WIN32

#ifndef SWDS
	//if(tevent) // TODO
		//CloseHandle(tevent); // TODO
#endif

#elif __linux__
	fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) & ~FNDELAY);
	fflush(stdout);
#endif

	exit(0);
};

double CSystem::GetFloatTime() const
{
	return Sys_FloatTime();
};