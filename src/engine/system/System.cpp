/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2021 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "System.hpp"
#include "ISystemImpl.hpp"
#include "CmdArgs.hpp"
#include "Memory.hpp"
#include "SystemEventDispatcher.hpp"

constexpr auto MAXPRINTMSG{4096};

#ifdef _WIN32
	#include "win/SystemWin.hpp"

	CSystemWin gSystemImpl;
#elif __unix__
	#include "unix/SystemUnix.hpp"
	
	CSystemUnix gSystemImpl;
#else
	#include "null/SystemNull.hpp"

	CSystemNull gSystemImpl;
#endif

CSystem gSystem(gSystemImpl);
//CSystem *gpSystem{&gSystem};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSystem, ISystem, OGS_SYSTEM_INTERFACE_VERSION, gSystem);

CSystem::CSystem(ISystemImpl &aImpl) : mImpl(aImpl)
{
	//mpEventDispatcher = std::make_unique<CSystemEventDispatcher>();
};

CSystem::~CSystem() = default;

void CSystem::Init(const char *asCmdLine)
{
	mpCmdLine = std::make_unique<CCmdArgs>(this, asCmdLine);
	
	int nMemSize{512 * 1024 * 1024}; // 512 Mb heap size
	
	// TODO: check for '-heapsize' arg
	
	mpMemory = std::make_unique<CMemory>(this, nMemSize);
	
	mImpl.Init();
};

//void CSystem::Shutdown()
//{
	// TODO
//};

void CSystem::AddEventListener(ISystemEventListener *apListener)
{
	mpEventDispatcher->AddListener(apListener);
};

void CSystem::RemoveEventListener(ISystemEventListener *apListener)
{
	mpEventDispatcher->RemoveListener(apListener);
};

void CSystem::DebugLog(const char *file, const char *fmt, ...)
{
	// TODO: Sys_DebugLog(file, fmt);
};

void CSystem::Error(const char *error, ...)
{
	va_list argptr;
	char text[1024]{};

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);

	mpEventDispatcher->DispatchError(text);
	
	mImpl.Error(text);
};

// TODO: temp
bool con_debuglog = true;
void Con_DebugLog(const char *asFileName, const char *asMsg, ...)
{
};

// NOTE: skipclient set to true will result in a legacy behavior 
// (as Sys_Printf instead of Con_Printf)
// (skipclient == true) = Con_Printf
// (skipclient == false) = Sys_Printf
void CSystem::Printf(const char *fmt, ...)
{
	va_list argptr;
	char text[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);
	
	mImpl.Printf("%s", text);
	
	// write it to the scrollable buffer on client-side
	//if(!skipclient) // TODO
		//if(gpEngineClient) // TODO
			//gpEngineClient->ConPrint(text); // TODO
	
	// log all messages to file
	if(con_debuglog)
		Con_DebugLog("qconsole.log", "%s", text);
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
	//if(!developer.GetValue()) // TODO
		//return;
	
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

void CSystem::Quit()
{
	//Host_Shutdown(); // TODO

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
	return mImpl.GetFloatTime();
};

double CSystem::GetRealTime() const
{
	return 0.0; //return realtime; // TODO
};

void CSystem::LowFPPrecision()
{
};

void CSystem::HighFPPrecision()
{
};