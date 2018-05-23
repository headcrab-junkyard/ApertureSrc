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
/// @brief IGameUIFuncs interface implementation

#include "quakedef.h"
#include "IGameUIFuncs.hpp"
#include "interface.h"

bool gbDedicatedServer{false};

char *gsPostRestartCmdLineArgs{nullptr};

int RunListenServer(void *instance, const char *basedir, const char *cmdline, char *postRestartCmdLineArgs, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory)
{
	// TODO: Whole bunch of Sys_Init* calls
	
	// TODO: CWindowManager::CreateGameWindow(); // IGame::CreateGameWindow
	
	if(!gpEngine->Load(false, basedir, cmdline))
		return EXIT_FAILURE;
	
	while(true)
		gpEngine->Frame();
	
	gpEngine->Unload();
	
	// TODO: CWindowManager::DestroyGameWindow(); // IGame::DestroyGameWindow
	
	// TODO: Whole bunch of Sys_Shutdown* calls
	
	return EXIT_SUCCESS;
};

class CEngineAPI final : public IEngineAPI
{
public:
	CEngineAPI();
	~CEngineAPI();
	
	int Run(void *instance, const char *basedir, const char *cmdline, char *postRestartCmdLineArgs, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CDedicatedServerAPI final : public IDedicatedServerAPI
{
public:
	CDedicatedServerAPI();
	~CDedicatedServerAPI();
	
	bool Init(const char *basedir, const char *cmdline, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory) override;
	int Shutdown() override;

	bool RunFrame() override;

	void AddConsoleText(const char *text) override;

	void UpdateStatus(float *fps, int *nActive, int *nMaxPlayers, const char *pszMap) override;
};

bool CDedicatedServerAPI::Init(const char *basedir, const char *cmdline, CreateInterfaceFn launcherFactory, CreateInterfaceFn filesystemFactory)
{
	if(!gpEngine->Load(true, basedir, cmdline))
		return false;
	
	return true;
};

bool CDedicatedServerAPI::RunFrame()
{
	//gpEngine->Frame();
	return false;
};

void CDedicatedServerAPI::AddConsoleText(const char *text)
{
	Cbuf_AddText(text);
};

void CDedicatedServerAPI::UpdateStatus(float *fps, int *nActive, int *nMaxPlayers, const char *pszMap)
{
	//Host_UpdateStatus(fps, nActive, nMaxPlayers, pszMap);
};