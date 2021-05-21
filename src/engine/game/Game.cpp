/*
Copyright (C) 2021 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "Game.hpp"
#include "System.hpp"

CGame::CGame(CSystem *apSystem) : mpSystem(apSystem){}

void CGame::Init()
{
	LoadModule();
	
	mpGame->Init(Sys_GetFactoryThis());
};

void CGame::Shutdown()
{
	if(mpGame)
	{
		mpGame->Shutdown();
		mpGame = nullptr;
	};
	
	UnloadModule();
};

void CGame::Update(float afTimeStep)
{
	mpGame->Update(afTimeStep);
};

void CGame::LoadModule()
{
	UnloadModule();
	
	mpGameModule = Sys_LoadModule(va("game"));
	
	if(!mpGameModule)
		mpSystem->Error("");
	
	auto fnGameFactory{Sys_GetFactory(mpGameModule)};
	
	if(!fnGameFactory)
		mpSystem->Error("");
	
	mpGame = reinterpret_cast<IGame*>(fnGameFactory(OGS_GAME_INTERFACE_VERSION, nullptr));
	
	if(!mpGame)
		mpSystem->Error("");
};

void CGame::UnloadModule()
{
	if(mpGameModule)
	{
		Sys_UnloadModule(mpGameModule);
		mpGameModule = nullptr;
	};
};