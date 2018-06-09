/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2015-2018 BlackPhrase
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

#include "Game.hpp"
#include "SystemEventListener.hpp"

IConsoleManager *gpConsoleManager{nullptr};

void DoABarrelRoll_f(const ICmdArgs &aArgs)
{
	static int nRolls{1};
	
	if(aArgs.GetCount() == 2)
		nRolls = atoi(aArgs.GetByIndex(2));
	
	for(auto i = 0; i < nCount; ++i)
		printf("Barrel roll!\n");
	
	printf("Do a barrel roll!\n");
};

EXPOSE_SINGLE_INTERFACE(CGame, IGame, MGT_GAME_INTERFACE_VERSION);

CGame::CGame() = default;
CGame::~CGame() = default;

bool CGame::Init(CreateInterfaceFn afnEngineFactory)
{
	// NOTE: Put your init routines here...
	
	mpSystem = (ISystem*)afnEngineFactory(MGT_SYSTEM_INTERFACE_VERSION, nullptr);
	
	gpCmdRegistry = (ICmdRegistry*)afnEngineFactory(MGT_CMDREGISTRY_INTERFACE_VERSION, nullptr);
	
	if(!mpSystem || !gpCmdRegistry)
		return false;
	
	// Add our system event listener here
	mpSystemEventListener = std::make_unique<CSystemEventListener>();
	mpSystem->AddEventListener(mpSystemEventListener.get());
	
	// Add entity event listener
	//mpEntityEventListener = std::make_unique<CEntityEventListener>();
	//mpSystem->AddEventListener(mpEntityEventListener.get());
	
	// Add game client event listener
	//mpGameClientEventListener = std::make_unique<CGameClientEventListener>();
	//mpServer->AddClientEventListener(mpGameClientEventListener.get());
	
	//CConCmd Cmd_DoABarrelRoll("barrelroll", DoABarrelRoll_f, FCVAR_EXTDLL, "Does a barrel roll (kinda)");
	//if(!gpCmdRegistry->Register(Cmd_DoABarrelRoll))
	if(!gpCmdRegistry->Add("barrelroll", DoABarrelRoll_f, "Does a barrel roll (kinda)"))
		return false;
	
	// Something game-specific...
	
	GameInit();
	RegisterEncoders();
	
	//mpServer->SetLagCompensationActive(true); // TODO
	
	//mpWorld->SetDefaultHulls(); // Setup default player hulls // TODO
	
	return true;
};

void CGame::Shutdown()
{
	// NOTE: Put your shutdown routines here...
	
	//mpServer->RemoveClientEventListener(mpGameClientEventListener.get());
	//mpWorld->RemoveEntityEventListener(mpEntityEventListener.get());
	mpSystem->RemoveEventListener(mpSystemEventListener.get());
};

void CGame::Update() // Frame
{
	// Per-frame logic update
};

const char *CGame::GetDescription() const
{
	return "Template";
};