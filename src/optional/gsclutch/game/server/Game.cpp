/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019, 2023 BlackPhrase
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

#include "Game.hpp"
#include "Server.hpp"
#include "GamePlayer.hpp"
#include "SystemEventListener.hpp"
#include "GameClientEventListener.hpp"

#include <next/engine/ISystem.hpp>
#include <next/engine/IGameServer.hpp>

ISystem *gpSystem{nullptr};

bool CGame::Init(CreateInterfaceFn afnEngineFactory)
{
	mpSystem = reinterpret_cast<ISystem*>(afnEngineFactory(OGS_SYSTEM_INTERFACE_VERSION, nullptr));
	mpGameServer = reinterpret_cast<IGameServer*>(afnEngineFactory(OGS_GAMESERVER_INTERFACE_VERSION, nullptr));
	
	if(!mpSystem || !mpGameServer)
		return false; // TODO: make more informative?
	
	gpSystem = mpSystem;
	
	// Connect the system event listener
	mpSystemEventListener = std::make_unique<CSystemEventListner>();
	mpSystem->AddListener(mpSystemEventListener.get());
	
	// Connect the game module as a game server event listener
	mpGameServer->AddListener(this);
	
	// Connect the game client event listener 
	mpGameClientEventListener = std::make_unique<CGameClientEventListener>(this);
	mpGameServer->AddClientListener(mpGameClientEventListener.get());
	
	// Init the legacy API game
	gEntityInterface.pfnGameInit();
	gEntityInterface.pfnRegisterEncoders();
	gEntityInterface.pfnPM_Init(gpsvmove); // TODO: add the sv move struct here
	return true;
};

void CGame::Shutdown()
{
	// Shutdown the legacy API game
	if(gNewDLLFunctions.pfnGameShutdown)
		gNewDLLFunctions.pfnGameShutdown();
};

void CGame::Frame(double afFrameTime)
{
	SV_Physics(afFrameTime);
	
	gEntityInterface.pfnStartFrame();
};

const char *CGame::GetDescription() const
{
	return gEntityInterface.pfnGetGameDescription();
};

void CGame::OnServerActivate(int anMaxEntities, int anMaxClients)
{
	// TODO: init the edicts array here
	mvEdicts = reinterpret_cast<edict_t*>(malloc(sizeof(edict_t) * anMaxEntities));
	
	// TODO: resize the players vector to anMaxClients size here
	mvPlayers.resize(anMaxClients);
	
	gEntityInterface.pfnServerActivate(mvEdicts, anMaxEntities, anMaxClients);
};

void CGame::OnServerDeactivate()
{
	gEntityInterface.pfnServerDeactivate();
};