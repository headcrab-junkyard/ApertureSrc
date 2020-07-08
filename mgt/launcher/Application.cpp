/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020 BlackPhrase
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

#include <stdexcept>

#include "Application.hpp"
#include "filesystem/IFileSystem.hpp"

CreateInterfaceFn gfnFSFactory{nullptr};

IBaseInterface *LauncherFactory(const char *name, int *retval)
{
	if(!strcmp(name, OGS_FILESYSTEM_INTERFACE_VERSION))
		return gfnFSFactory(name, retval);
	
	auto fnThisFactory{Sys_GetFactoryThis()};
	return fnThisFactory(name, retval);
};

CApplication::CApplication() = default;
CApplication::~CApplication() = default;

int CApplication::Run()
{
	do
	{
		if(!Init())
			return EXIT_FAILURE;
		
		bool bRunning{true};
		
		// main loop
		while(bRunning)
			bRunning = mpEngine->Frame();
		
		mbRestart = false; // TODO
		
		Shutdown();
	}
	while(mbRestart);
	
	// return success of application
	return EXIT_SUCCESS;
};

bool CApplication::Init()
{
	if(!LoadFileSystemModule("filesystem_stdio"))
		throw std::runtime_error(std::string("Failed to load the filesystem module!"));
	
	mpEngineLib = Sys_LoadModule("engine");
	
	if(!mpEngineLib)
		throw std::runtime_error(std::string("Failed to load the engine module!"));
	
	auto pEngineFactory{Sys_GetFactory(mpEngineLib)};
	
	if(!pEngineFactory)
		return false;
	
	mpEngine = (IEngine*)pEngineFactory(OGS_ENGINE_INTERFACE_VERSION, nullptr);
	
	if(!mpEngine)
		return false;
	
	IEngine::InitParams InitParams{};
	
	InitParams.sGameDir = ".";
	InitParams.sCmdLine = "";
	//InitParams.sPostRestartCmdLine = ""; // TODO
	InitParams.fnLauncherFactory = LauncherFactory;
	InitParams.bDedicated = false;
	
	if(!mpEngine->Init(InitParams))
		return false;
	
	return PostInit();
};

void CApplication::Shutdown()
{
	mpEngine->Shutdown();
};

bool CApplication::LoadFileSystemModule(const char *name)
{
	if(!name || !*name)
	{
		//throw std::invalid_argument(std::string("Argument passed to CApplication::LoadFileSystemModule is invalid! (").append(name).append(")"));
		return false;
	};
	
	mpFSLib = Sys_LoadModule(name);
	
	if(!mpFSLib)
		return false;
	
	gfnFSFactory = Sys_GetFactory(mpFSLib);
	
	if(!gfnFSFactory)
		return false;
	
	return true;
};