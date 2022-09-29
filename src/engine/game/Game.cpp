/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2021-2022 BlackPhrase
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
#include "Game.hpp"
#include "System.hpp"

#include <game/server/IGame.hpp>

CGame::CGame(CSystem *apSystem) : mpSystem(apSystem){}

bool CGame::Init()
{
	LoadModule();
	
	return mpGame->Init(Sys_GetFactoryThis());
};

void CGame::Shutdown()
{
	mpGame->Shutdown();
	
	UnloadModule();
};

void CGame::Update(float afTimeStep)
{
	mpGame->Frame(afTimeStep); // TODO: Update(afTimeStep);
};

void CGame::LoadModule()
{
	UnloadModule();
	
	mpGameModule = Sys_LoadModule(va("%s/bin/server", com_gamedir));
	
	if(!mpGameModule)
		mpSystem->Error("Failed to load the game module!");
	
	auto fnGameModuleFactory{Sys_GetFactory(mpGameModule)};
	
	if(!fnGameModuleFactory)
		mpSystem->Error("Failed to get the factory from the game module!");
	
	mpGame = reinterpret_cast<IGame*>(fnGameModuleFactory(OGS_GAME_INTERFACE_VERSION, nullptr));
	
	if(!mpGame)
		mpSystem->Error("Failed to get the game interface!");
};

void CGame::UnloadModule()
{
	if(mpGameModule)
	{
		Sys_UnloadModule(mpGameModule);
		mpGameModule = nullptr;
	};
};