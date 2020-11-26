/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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
/// @brief coordinates spawning and killing of local servers

#include "Host.hpp"
#include "Memory.hpp"
#include "CmdBuffer.hpp"
#include "Cmd.hpp"
#include "Cvar.hpp"
#include "Sys.hpp"
#include "Net.hpp"
#include "Progs.hpp"
#include "ModelCache.hpp"
#include "Server.hpp"

#include "engineclient/IEngineClient.hpp"
#include "game/server/IGame.hpp"

CHost::CHost(IEngineClient *apEngineClient) : mpEngineClient(apEngineClient){}

bool CHost::Init()
{
	mpMemory->Init(nullptr, 0); // TODO
	mpCmdBuffer->Init();
	mpCmd->Init();
	mpCvar->Init();
	
	//Host_InitVCR(parms); // TODO ?
	//COM_Init(); // TODO
	
	//mpSystem->Init(); // TODO ?
	
	InitLocal();
	
	mpNetwork->Init();
	//Netchan_Init(); // TODO
	
	mpModelCache->Init();
	
	mpServer->Init();
	
	if(!InitClient())
		return false;
	
	LoadGameModule();
	
	mpProgs->Init();
	
	if(mpGame->Init(Sys_GetFactoryThis()))
		return false;
	
	return true;
};

/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void CHost::Shutdown()
{
	static bool bDown{false};

	if(bDown)
	{
		printf("recursive shutdown\n");
		return;
	};

	bDown = true;
	
	ShutdownClient();
	
	mpGame->Shutdown();
	
	UnloadGameModule();
	
	//mpServer->Shutdown(); // TODO
	
	mpNetwork->Shutdown();
	
	mpSystem->Quit();
};

bool CHost::Frame(float afTime)
{
	static double fTimeTotal{0.0};
	static int nTimeCount{0};
	
	bool bProfile = false; // TODO: host_profile.GetValue()
	if(!bProfile)
	{
		_Frame(afTime);
		return true;
	};
	
	double fTimePreFrame = mpSystem->GetDoubleTime();
	_Frame(afTime);
	double fTimePostFrame = mpSystem->GetDoubleTime();

	fTimeTotal += fTimePostFrame - fTimePreFrame;
	nTimeCount++;

	if(nTimeCount < 1000)
		return false;

	int m = fTimeTotal * 1000 / nTimeCount;
	
	nTimeCount = 0;
	fTimeTotal = 0.0;

	mpSystem->Printf("host_profile: %2i clients %2i msec\n", mpServer->GetActiveClientsNum(), m);
	return true;
};

/*
==================
Host_Frame

Runs all active servers
==================
*/
bool CHost::_Frame(float afTime)
{
	// keep the random time dependent
	rand();
	
	// process console commands
	mpCmdBuffer->Execute();
	
	//-------------------
	//
	// server operations
	//
	//-------------------
	
	mpServer->Frame(afTime);
	
	mpGame->Frame(afTime);
	
	//-------------------
	//
	// client operations
	//
	//-------------------
	
	if(mpEngineClient)
	{
		if(!mpEngineClient->PreFrame())
			return false;
		
		mpEngineClient->Frame();
	};

	++mnFrameCount;
	return true;
};

void CHost::InitLocal()
{
};

bool CHost::InitClient()
{
	if(mpEngineClient)
		if(mpEngineClient->Init(Sys_GetFactoryThis()))
			return true;
	
	return false;
};

void CHost::ShutdownClient()
{
	if(mpEngineClient)
		mpEngineClient->Shutdown();
};

void CHost::LoadGameModule()
{
	mpGameModule = Sys_LoadModule("./sandbox/bin/server" /*va("%s/bin/server", com_gamedir)*/); // TODO
	
	if(!mpGameModule)
		mpSystem->Error("Failed to load the game module!");
	
	auto pfnGameModuleFactory{Sys_GetFactory(mpGameModule)};
	
	if(!pfnGameModuleFactory)
		mpSystem->Error("Failed to get the factory from the game module!");
	
	mpGame = reinterpret_cast<IGame*>(pfnGameModuleFactory(OGS_GAME_INTERFACE_VERSION, nullptr));
	
	if(!mpGame)
		mpSystem->Error("Failed to get the game interface from the game module!");
};

void CHost::UnloadGameModule()
{
	Sys_UnloadModule(mpGameModule);
};