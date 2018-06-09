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

#include <cstdio>
#include <stdexcept>

#ifdef _WIN32
	#include <windows.h>
	#include "win/conproc.h"
#endif

#include "engine/IEngine.hpp"
#include "interface.h"

CreateInterfaceFn gfnFSFactory{nullptr};

IBaseInterface *LauncherFactory(const char *name, int *retval)
{
	if(!strcmp(name, MGT_FILESYSTEM_INTERFACE_VERSION))
		return gfnFSFactory;
	
	return Sys_GetFactoryThis();
};

bool InitConsole()
{
// TODO
/*
#ifdef _WIN32
	if(!AllocConsole())
		Sys_Error("Couldn't create dedicated server console");

	hinput = GetStdHandle (STD_INPUT_HANDLE);
	houtput = GetStdHandle (STD_OUTPUT_HANDLE);

	// give QHOST a chance to hook into the console
	if ((t = COM_CheckParm ("-HFILE")) > 0)
	{
		if (t < com_argc)
			hFile = (HANDLE)Q_atoi (com_argv[t+1]);
	};
		
	if ((t = COM_CheckParm ("-HPARENT")) > 0)
	{
		if (t < com_argc)
			heventParent = (HANDLE)Q_atoi (com_argv[t+1]);
	};
		
	if ((t = COM_CheckParm ("-HCHILD")) > 0)
	{
		if (t < com_argc)
			heventChild = (HANDLE)Q_atoi (com_argv[t+1]);
	};

	InitConProc (hFile, heventParent, heventChild);
#endif // _WIN32
*/
};

int RunServer() // void?
{
	auto pFSLib{Sys_LoadModule("filesystem_stdio")};
	
	if(!pFSLib)
		throw std::runtime_error(std::string("Failed to load the filesystem module!"));
	
	auto pFSFactory{Sys_GetFactory(pFSLib)};
	
	if(!pFSFactory)
		return EXIT_FAILURE;
	
	auto pEngineLib{Sys_LoadModule("engine")};
	
	if(!pEngineLib)
		throw std::runtime_error(std::string("Failed to load the engine module!"));
	
	auto pEngineFactory{Sys_GetFactory(pEngineLib)};
	
	if(!pEngineFactory)
		return EXIT_FAILURE;
	
	auto pEngine{(IEngine*)pEngineFactory(MGT_ENGINE_INTERFACE_VERSION, nullptr)};
	
	if(!pEngine)
		return EXIT_FAILURE;
	
	IEngine::SInitData InitParams{};
	
	InitParams.sGameDir = ".";
	InitParams.sCmdLine = "";
	InitParams.fnLauncherFactory = LauncherFactory;
	//InitParams.fnFSFactory = pFSFactory;
	InitParams.bDedicated = true;
	
	if(!pEngine->Init(InitParams))
		return EXIT_FAILURE;
	
	while(true)
		pEngine->Frame();
	
	pEngine->Shutdown();
	
	return EXIT_SUCCESS;
};

int main(int argc, char **argv)
{
	//CCmdLine CmdLine(argc, argv); // TODO
	
	if(!InitConsole())
		return EXIT_FAILURE;
	
	if(!RunServer())
		return EXIT_FAILURE;
	
#ifdef _WIN32
	FreeConsole();
	
	// shutdown QHOST hooks if necessary
	//DeinitConProc(); // TODO
#endif

	// return success of application
	return EXIT_SUCCESS;
};